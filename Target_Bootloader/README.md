# Target_Bootloader — STM32F103C8 UART Bootloader

This is the ARM-side heart of the FOTA system: a bare-metal, self-flashing bootloader
for the STM32F103C8 (Cortex-M3) that lives in the first 32 KB of flash, decides whether
to jump into the [user application](../Application) or wait for new firmware, and — when
told to — receives a new binary over UART and writes it into flash itself. It is the
counterpart the Raspberry Pi's [C++ host bootloader](../CPP_Application) talks to.

See the [repository root README](../README.md) for the end-to-end OTA picture, and
[Application/README.md](../Application/README.md) for the sibling project this
bootloader hands control to.

## Layout

```
Target_Bootloader/
├── CMakeLists.txt                Generic STM32F103C8 CMake build (arm-none-eabi-gcc)
├── CleanBuildDir.cmake           POST_BUILD helper: keeps only the latest build output
├── Diagrams/
│   └── Baremetal_Bootloader.puml Sequence diagram of the boot decision flow
├── Include/
│   ├── Application/Baremetal_Bootloader.h   Entry-point app config + prototypes
│   ├── Library/
│   │   ├── Standard_Types.h / Macros.h      Same as Application (see its README)
│   │   ├── MCU/STM32F103.h                  Peripheral base addresses, register maps
│   │   ├── Cortex_M3/{Headers,Linker_Script.ld}  CMSIS + STM32F103C8Tx memory map
│   │   └── Bootloader/
│   │       ├── Bootloader_Interface.h       Protocol, states, commands, memory layout
│   │       ├── Version_Config.h.in          Template CMake fills in with Chip_ID/Major/Minor
│   │       └── Bootloader.sh                Manual UART protocol test script (host-side)
│   └── MCAL/
│       ├── GPIO, RCC, EXTI, NVIC            Same drivers as Application
│       ├── CRC/CRC.h                        Hardware CRC-32 unit driver
│       ├── FLASH/FLASH.h                    Self-programming flash driver
│       └── USART/USART_Interface.h          UART driver (interrupt + polling)
└── Source/
    ├── Application/Baremetal_Bootloader.c   main(): boot-indicator + Bootloader_Start()
    ├── Library/
    │   ├── Cortex_M3/{Startup.s,System_Init.c}  Identical to Application's
    │   └── Bootloader/Bootloader_Program.c  The bootloader state machine (~660 lines)
    └── MCAL/{GPIO,RCC,EXTI,NVIC,CRC,FLASH,USART}/*.c
```

## Architecture

Same three-tier **Library → MCAL → Application** layering as
[Application/](../Application), plus one extra tier sitting between MCAL and
Application:

| Layer | Directory | Responsibility |
|---|---|---|
| **Library** | `Library/` | Toolchain/chip/core plumbing — identical to Application's (`Startup.s` and `RCC_Program.c` are byte-for-byte the same file). |
| **MCAL** | `MCAL/` | Register-level peripheral drivers. GPIO/RCC/EXTI/NVIC are shared with Application; **CRC**, **FLASH**, and **USART** are added here because only the bootloader needs them. |
| **Bootloader** (protocol layer) | `Library/Bootloader/` | The actual bootloader logic: the boot decision (jump vs. wait), the UART command protocol, CRC-gated flashing. This is a `Library` module rather than `MCAL` because it's chip-independent protocol/business logic built *on top of* the MCAL drivers, not a peripheral driver itself. |
| **Application** | `Application/` | The thin entry point: boot-indicator LEDs, a fixed delay window, then hands off to `Bootloader_Start()`. |

## Component details

### Library (shared with Application)

`Standard_Types.h`, `Macros.h`, `MCU/STM32F103.h`, the CMSIS headers, `Startup.s`, and
`System_Init.c` are the same files (or line-for-line identical logic) as in
[`Application/`](../Application) — see that project's README for what each does. One
addition here: `STM32F103.h` also defines `UNIQUE_DEVICE_ID_REGISTER` (the 96-bit factory
unique ID at `0x1FFFF7E8`, used by the `Send_ID` command) and the `SCB` struct/macro used
for the software-reset sequence.

### MCAL — GPIO / RCC / EXTI / NVIC

Functionally identical to Application's drivers (`RCC_Interface.h` is byte-identical;
`GPIO_Interface.h`/`GPIO_Program.c` differ only in which headers they `#include`, not in
behavior).

### MCAL — CRC (`CRC.h` / `CRC.c`)

Thin wrapper around the STM32F1 hardware CRC-32 unit (polynomial `0x04C11DB7`, the
default STM32 CRC-32/MPEG-2-style unit):

- `CRC_Initialization()` — enables the CRC peripheral clock (`RCC_Clock(RCC_CRC, Enable)`) and resets it.
- `CRC_Reset()` — sets `CRC.CR.Bit_0` to reset the running CRC accumulator to its initial value.
- `CRC_Accumulate(u32 Value)` — writes one 32-bit word into `CRC.DR` and returns the updated running CRC. Called repeatedly to fold a whole buffer into one CRC value.

This single hardware unit is reused for **two different purposes** in the bootloader
(see below): validating each UART frame, and validating the entire flashed application
image.

### MCAL — FLASH (`FLASH.h` / `FLASH.c`)

Self-programming driver for the STM32F1 embedded flash controller:

- `FLASH_Unlock()` / `FLASH_Lock()` — the standard two-key unlock sequence (`KEY1 =
  0x45670123`, `KEY2 = 0xCDEF89AB` written to `FLASH.KEYR`), required before any
  erase/program operation; the driver always re-locks flash when it's done.
- `Flash_Erase_Page(u32 pageAddress)` *(static)* — sets the Page-Erase bit, writes the
  target page address to `FLASH.AR`, starts the erase, busy-waits on `FLASH.SR.Bit_0`
  (BSY), and checks the programming-error/write-protect-error flags afterward.
- `Flash_Erase_Pages(u8 Start_Page, u8 Pages_Number)` — public entry point: unlocks
  flash once, loops `Flash_Erase_Page()` over a page range (1 KB pages, `Flash_Page_Size
  = 0x400`), then re-locks.
- `FLASH_Halfword_Write(u32 Address, u16 Data)` *(static)* — the STM32F1 flash controller
  only programs 16 bits at a time; this sets the programming bit, writes the halfword,
  waits for completion, and **reads the value back to verify it actually landed**.
- `Flash_Write_Page(u8 Page_Number, u32 Data[])` — writes a full 1 KB page (256 × 32-bit
  words) as 512 halfword writes.
- `Flash_Write_Data(u32 Address, u32 Data)` — writes a single 32-bit word (two
  halfwords) at an arbitrary address, used by the `Send_Data` command for one-off
  patches without a full page erase/rewrite.

**Why this matters architecturally:** on the STM32F1, code cannot reliably execute out
of the flash bank that is currently being erased/programmed. The linker script places
`Flash_Erase_Pages`, `Flash_Write_Page`, `Flash_Write_Data`, `Bootloader_Erase_Flash`,
`Bootloader_Write_Flash`, and `Bootloader_Send_Data` into a custom `.FLASH_OPERATIONS`
section, which the linker script loads into **RAM** (`>RAM AT> FLASH`, appended right
after `.data`) instead of leaving them in flash. So the actual flash-programming code
runs from RAM while it's busy erasing/rewriting the flash it's stored in — this is what
lets the bootloader safely re-flash its own chip's application region.

### MCAL — USART (`USART_Interface.h` / `USART_Program.c`)

Interrupt-capable UART driver:

- `USART_Config_t` bundles everything about one UART instance: port number
  (`USART_1/2/3`), parity, mode (Rx/Tx/both), flow control, word length, stop bits,
  baud rate, and an optional interrupt mode with a callback pointer
  (`void (*Call_Back_Function)(u16 *Data)`).
- `USART_Initialization` / `USART_Reset` — bring the peripheral up/down (clock, GPIO
  alternate-function pins, baud-rate register, NVIC interrupt enable if configured).
- `USART_Transmit` / `USART_Receive` — single 8/9-bit word, blocking.
- `USART_Send_Array` / `USART_Send_String` — helpers built on `USART_Transmit`.

The bootloader uses `USART_1` at **115200 baud, 8N1**, and switches between interrupt
mode (while waiting during the boot window) and polling mode (once actively receiving a
command) — see below.

### Bootloader protocol layer (`Bootloader_Interface.h` / `Bootloader_Program.c`)

This is the actual bootloader: a ~660-line state machine built entirely on the MCAL
drivers above. Its two jobs are (1) **decide** whether to run the application or wait
for a new one, and (2) **serve** a small command protocol over UART that can inspect the
chip, erase flash, and write a new application image.

#### Boot decision (`Bootloader_Start`)

```c
void Bootloader_Start(void) {
    while(1) {
        if (Open_Bootloader) {
            Bootloader_Initialize();
            while(1) { Bootloader_Receive_Command(); }
        } else if (Validate()) {
            USART_Reset(&Bootloader_UART);
            Bootloader_Start_Application(Application_Base);
        } else {
            Start_Bootloader_Interrupt(&Dummy_Variable);   // forces Open_Bootloader = TRUE
        }
    }
}
```

The actual sequence, reading `Baremetal_Bootloader.c`'s `main()` together with this
function:

1. `System_Initialization()` configures the boot-indicator LED pins and calls
   `Bootloader_Initialize()`, which brings up USART1 **in interrupt mode** with
   `Start_Bootloader_Interrupt` as the RX-complete callback, and initializes the CRC unit.
2. `Test()` drives all four LEDs (`PB0/1/14/15`) high — the visible "bootloader is
   alive and listening" indicator.
3. `delay_ms(4000)` — a 4-second window. If the Raspberry Pi host sends **any** byte
   during this window, the USART RX interrupt fires, `Start_Bootloader_Interrupt` runs
   and sets `Open_Bootloader = TRUE` (and switches USART to polling mode, disabling the
   interrupt so subsequent bytes are read synchronously by the command loop instead).
   This is the mechanism behind the sequence diagram's "Request Halt" / ACK step: any
   byte from the host during the window counts as a halt request.
4. `Bootloader_Start()` runs its loop:
   - If `Open_Bootloader` is already `TRUE` (host interrupted the wait) → re-initialize
     and loop forever on `Bootloader_Receive_Command()` — the command-server state.
   - Else if `Validate()` (a full hardware-CRC pass over the application region, see
     below) says the flashed application is intact → reset USART and
     `Bootloader_Start_Application()` jumps straight into it, **no further host
     interaction required**.
   - Else (no valid application present, e.g. a blank or corrupted chip) → the code
     unconditionally calls `Start_Bootloader_Interrupt()` itself, forcing
     `Open_Bootloader = TRUE` regardless of whether the host ever sent anything. This is
     a fail-safe: a chip with no valid application always falls into the command server
     rather than getting stuck, so it can always be recovered by flashing a new image.

The [`Diagrams/Baremetal_Bootloader.puml`](Diagrams/Baremetal_Bootloader.puml) sequence
diagram documents this at a higher level (Halt ACK → command loop, vs. NACK/timeout →
jump to application); the CRC-fail-safe fallback described above isn't shown in the
diagram but is implemented in code.

#### Application integrity check (`Validate`)

```c
bool Validate(void) {
    CRC_Reset();
    for (Iterator = Application_Base; Iterator < End_Memory; ++Iterator)
        CRC_Result = CRC_Accumulate(*Iterator);
    return CRC_Result == Valid_CRC;
}
```

Runs the hardware CRC-32 unit over **every word from `Application_Base` (flash offset
32 KB) to `End_Memory` (flash offset 64 KB)** — the entire application region — and
compares it against `Valid_CRC`, a 32-bit value stored at fixed flash address
`0x8007FE0` (the `.APPLICATION_CRC` linker section). Whoever flashes a new application
is responsible for also writing its expected CRC to that address (e.g. via the
`Send_Data` command) so this check has something correct to compare against.

#### UART frame format & per-frame CRC (`Bootloader_Receive_Command` / `Bootloader_CRC_Check`)

Every command frame the host sends looks like:

```
[ Length ][ Command ][ ...Payload... ][ CRC32 (4 bytes) ]
```

- **Length** (1 byte) — total byte count of everything *after* the length byte,
  including the trailing CRC.
- **Command** (1 byte) — a `Bootloader_Command_t` value.
- **Payload** — command-specific (see table below).
- **CRC32** (4 bytes, big-endian as accumulated 32-bit words) — computed by the host
  over `[Length][Command][Payload]` using the same CRC-32 algorithm as the STM32
  hardware unit.

`Bootloader_Receive_Command()` blocking-reads the length byte, then reads exactly that
many more bytes into `UART_Buffer` (capped at `Maximum_Buffer_Size = 255`).
`Bootloader_CRC_Check()` then re-accumulates the CRC over the buffer (excluding the last
4 CRC bytes) 32 bits at a time and compares it against the CRC the host appended; on
mismatch the bootloader replies NACK and drops the frame without executing anything.
Only on a CRC match does `Bootloader_Check_Command()` dispatch on the command byte.

#### Command set

| Command | Value | Handler | What it does |
|---|---|---|---|
| `Send_Help` | 1 | `Bootloader_Send_Help` | Replies with the list of supported command bytes. **Note:** the reply only lists 6 of the 9 defined commands (`Help, ID, Version, Erase, Flash, Jump`) — `Say_Hi`, `Say_Bye`, and `Send_Data` are implemented and dispatchable but omitted from this help list. |
| `Send_ID` | 2 | `Bootloader_Send_ID` | Sends the 96-bit factory-programmed unique device ID (12 bytes) from `UNIQUE_DEVICE_ID_REGISTER`. |
| `Send_Version` | 3 | `Bootloader_Send_Version` | Sends 3 bytes (Chip ID, Major, Minor) read back from the `.VERSION` flash section at `0x8007FFC`, written at build time via `SET_VERSION()` — see **Versioning** below for where these numbers actually come from. |
| `Erase_Flash` | 4 | `Bootloader_Erase_Flash` | Payload: start page + page count. Only allowed for pages `>= 31` and `< 128` (i.e. restricted to the application region); erases via `Flash_Erase_Pages`, replies ACK/NACK. |
| `Flash_Application` | 5 | `Bootloader_Write_Flash` | The main OTA write path — see below. |
| `Address_Jump` | 6 | `Bootloader_Address_Jump` | Payload: a 32-bit address. If it falls inside the chip's flash range, ACKs and calls `Bootloader_Start_Application()` at that address (lets a host jump to an arbitrary valid image without going through `Validate()`). |
| `Say_Hi` | 7 | `Bootloader_Say_Hi` | ACK + empty frame — simple liveness/handshake check. |
| `Say_Bye` | 8 | `Bootloader_Say_Bye` | ACK + empty frame, then performs a software reset of the MCU (`SCB->AIRCR` reset key) — lets a host end a session and force a clean reboot back through the boot-decision logic. |
| `Send_Data` | 9 | `Bootloader_Send_Data` | Payload: 32-bit address + 32-bit data word. Writes one word anywhere in the chip's flash range via `Flash_Write_Data` — used, among other things, to patch the `.APPLICATION_CRC` word after flashing a new application. |

Replies use `Bootloader_State_t`: `ACK(1)`, `NACK(2)`, `Successful_Erase(3)`,
`Unsuccessful_Erase(4)`, `Successful_Write(5)`, `Unsuccessful_Write(6)`,
`Error_CRC(7)`, `Wrong_Command(8)` (state `OK = 0` is used internally, never sent).

#### Flashing a new application (`Bootloader_Write_Flash`)

1. Payload gives a starting page and a total frame count; the function first computes
   how many 1 KB pages the incoming image needs (`(250 * Total_Payload_Frames)/1024 +
   1`, since each subsequent payload frame carries up to 250 data bytes) and erases
   exactly that many pages up front.
2. ACKs the erase, then loops receiving one CRC-checked frame at a time
   (`Bootloader_Receive_Payload`, which reuses the same length+CRC framing described
   above), copying each frame's payload bytes into a 1 KB RAM staging buffer
   (`Page_Buffer`).
3. Whenever the staging buffer fills (or the last frame's last byte arrives),
   `Flash_Write_Page()` commits that page to flash, the buffer is memset back to
   `0xFF` (erased-flash value), and the write advances to the next page.
4. Every accepted frame gets its own ACK; a CRC failure on any frame gets a NACK (the
   loop does **not** abort on a bad frame — it keeps waiting for the same frame index
   again, so a flaky link can retry mid-transfer rather than failing the whole image).

### Application entry point (`Baremetal_Bootloader.h` / `.c`)

The thinnest layer in the project — just wires the pieces above together:

- `System_Initialization()` — configures the 4 boot-indicator LED pins on `GPIO_B`
  (0, 1, 14, 15) and calls `Bootloader_Initialize()`.
- `Test()` — drives all 4 LEDs high (visual "bootloader active" signal — see boot
  decision flow above).
- `main()` — `System_Initialization(); Test(); delay_ms(4000); Bootloader_Start();`

### `Bootloader.sh`

A standalone bash helper (`Include/Library/Bootloader/Bootloader.sh`) for manually
exercising the UART protocol from a Linux host without the full C++ host application —
useful for bring-up/debugging. It opens a `screen` session logging to a file on
`/dev/ttyUSB0` at 9600 baud, and provides a menu-driven loop; only "Get Help" is fully
wired up (`Get_Help()` builds a length+command+CRC32 frame — computed via a Python
`crccheck.crc.Crc32Mpeg2` one-liner — and sends it with `socat`), the other menu options
are stubs (`break` with no frame construction yet). **Note:** this script targets
9600 baud and `/dev/ttyUSB0` (a USB-serial adapter), while the firmware itself configures
`USART_115200` — the baud rate would need to match before this script could actually talk
to the board.

## Memory map

The linker script (`Include/Library/Cortex_M3/Linker_Script.ld`) is the source of truth,
and matches the constants in `Bootloader_Interface.h`:

| Region | Address range | Size | Purpose |
|---|---|---|---|
| Bootloader code | `0x08000000` – `0x08007FFF` | 32 KB (pages 0–31) | This project's `.isr_vector`/`.text`/`.rodata` — matches `FLASH ORIGIN = 0x08000000, LENGTH = 32K` and `Bootloader_Total_Pages = 32`. |
| Application region | `0x08008000` – `0x0800FFFF` | 32 KB (pages 32–63, per `Memory_Size = 0x10000`) | `Application_Base`/`Application_Page = 32`, `End_Memory = Memory_Base + Memory_Size`; exactly where [`Application/`](../Application)'s own linker script places itself (`FLASH ORIGIN = 0x08008000`). |
| `.VERSION` | `0x08007FFC` | 4 bytes | Chip ID / Major / Minor, packed via `SET_VERSION()`; read by the `Send_Version` command. |
| `.APPLICATION_CRC` | `0x08007FE0` | 4 bytes | Expected CRC-32 of the application region, checked by `Validate()`. |
| RAM | `0x20000000` – `0x20004FFF` | 20 KB | Stack top `_estack = 0x20005000`; also hosts the `.FLASH_OPERATIONS` code copied here at startup (see FLASH driver section above). |

> **Note on page/erase-range checks:** `Bootloader_Erase_Flash` accepts pages `>= 31`
> (not `32`) through `< 128`, and `Memory_Size = 0x10000` (64 KB) implies the chip's
> *total* flash is 64 KB (pages 0–63) — but the erase-range check and
> `Flash_Maximum_Pages = 128` both assume a 128 KB part. The STM32F103**C8** actually has
> 64 KB of flash; the `CMakeLists.txt` `read` target likewise assumes `FLASH_Size =
> 131072` (128 KB). These are pre-existing inconsistencies between the code's assumed
> and the real STM32F103C8 flash size — worth resolving before relying on the upper
> half of that address range.

## Versioning

Unlike a build-only version tag, the bootloader's version is **functionally meaningful**:
it's baked into the flashed image and served back over UART by the `Send_Version`
command, so a host can query which firmware a chip is running. `CMakeLists.txt` defines
the three numbers that matter:

```cmake
#Version Control
set(Chip_ID 1)
set(Major 1)
set(Minor 0)
```

At configure time, `configure_file()` renders
[`Include/Library/Bootloader/Version_Config.h.in`](Include/Library/Bootloader/Version_Config.h.in)
into `<build>/Version_Config.h`, substituting these three values into
`BOOTLOADER_CHIP_ID` / `BOOTLOADER_SW_MAJOR` / `BOOTLOADER_SW_MINOR`.
[`Bootloader_Interface.h`](Include/Library/Bootloader/Bootloader_Interface.h) includes
that generated header and defines `Default_Chip_ID_Number` /
`Default_SW_Major_Version` / `Default_SW_Minor_Version` from it (previously these were
hardcoded placeholder `250`s). `Bootloader_Program.c` then bakes them into the flash
image via `SET_VERSION()` into the `.VERSION` linker section, unchanged from before —
only where the three numbers come from changed. **To bump the bootloader's version,
edit `Chip_ID`/`Major`/`Minor` in `CMakeLists.txt` and rebuild** — no source-file edits
needed.

The binary's *file name*, by contrast, is not a version at all — see below.

## Build & flash

```bash
mkdir build && cd build
cmake ..
make                # -> Baremetal_Bootloader_<Chip_ID>_<YYYYMMDD>_<HHMMSS>{,.bin,.hex,.axf,.map}
make flash          # st-flash write <bin> 0x08000000
make erase          # st-flash erase
make read           # st-flash read FLASH.bin 0x08000000 131072
```

Same toolchain/flags as [Application](../Application) (`arm-none-eabi-gcc`, Cortex-M3,
nano/nosys specs; the same invalid `-g2` assembler flag that broke Application's build
has been removed here too). Two behaviors now mirror Application's build:

- **Auto-generated, timestamped binary name** — `Application_Name` is
  `Baremetal_Bootloader_<Chip_ID>_<Build_Date>_<Build_Time>` (e.g.
  `Baremetal_Bootloader_1_20260905_103244`), computed fresh every `cmake` configure via
  `string(TIMESTAMP ...)`. This identifies *which build* produced a binary; it is
  independent from the `Major`/`Minor` **firmware version** described above.
- **Old-output cleanup** — a POST_BUILD step runs
  [`CleanBuildDir.cmake`](CleanBuildDir.cmake) after every successful build, deleting
  every earlier `Baremetal_Bootloader_*` artifact in the build directory and keeping
  only the one just built (implemented via CMake script mode / `file(GLOB...)`, not a
  shell `find` command — Make runs custom-command recipes through a shell that
  glob-expands unquoted wildcards before a naive `find`/`bash -c` one-liner would see
  them, which is a real trap here).

There's still no VS Code copy-to-`Build/` step for this project (unlike Application's
task, which stages its `.bin` for pickup elsewhere).

## Relationship to the rest of the repo

- **[`Application/`](../Application)** — the image this bootloader jumps to (or
  re-flashes). Its linker script's flash origin (`0x08008000`) and this bootloader's
  `Application_Base` agree by construction, not by any shared build-time check — moving
  one without the other would silently break the handoff.
- **[`CPP_Application/Bootloader`](../CPP_Application)** — the Raspberry Pi host
  implementing the other end of this exact command protocol (`Bootloader_Interface.hpp`
  mirrors the enums in `Bootloader_Interface.h` by hand): it holds the target in reset
  or sends a byte during the 4-second window to force `Open_Bootloader`, then drives the
  erase/flash/jump command sequence described above.
- **[`Diagrams/Baremetal_Bootloader.puml`](Diagrams/Baremetal_Bootloader.puml)** — the
  high-level sequence view of the boot decision flow described in detail above.
