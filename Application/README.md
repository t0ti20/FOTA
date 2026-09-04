
This is the **user application** half of the FOTA target firmware: a minimal, layered bare-metal program for the STM32F103C8 ("Blue Pill", Cortex-M3) that demonstrates the code the bootloader hands control to after an OTA update. On its own it does one thing — blink LEDs on GPIO port B but it exists to prove the bootloader application handoff (reset vector, memory layout, jump address) actually works end-to-end.

See the repository root README for how this fits into the full OTA pipeline (Raspberry Pi host ↔ UART ↔ target bootloader ↔ this application).

## Layout

```
Application/
├── CMakeLists.txt              Generic STM32F103C8 CMake build (arm-none-eabi-gcc)
├── Include/
│   ├── Application/             Application.h            — app config + prototypes
│   ├── Library/
│   │   ├── Standard_Types.h     u8/u16/u32/u64, f32/f64  fixed-width aliases
│   │   ├── Macros.h             logic_t, bit-math macros, delay_ms()
│   │   ├── MCU/STM32F103.h      Peripheral base addresses, bit-band macro, Bits_t
│   │   └── Cortex_M3/
│   │       ├── Headers/         CMSIS core + ST headers (core_cm3.h, ...)
│   │       └── Linker_Script.ld STM32F103C8Tx memory map (see below)
│   └── MCAL/
│       ├── GPIO/GPIO_Interface.h    Pin/port config, read/write/toggle API
│       ├── RCC/RCC_Interface.h      Clock-enable API for every peripheral bus
│       ├── EXTI/EXTI_Interface.h    External-interrupt configuration API
│       └── NVIC/NVIC_Interface.h    IRQ enable/disable API + IRQ number table
└── Source/
    ├── Application/Application.c    main(), System_Init(), Test_Blue/Yellow()
    ├── Library/Cortex_M3/
    │   ├── Startup.s                Vec table + Reset_H (.data/.bss init, → main)
    │   └── System_Init.c            SystemInit()/SystemCoreClock() (ST CMSIS file)
    └── MCAL/{GPIO,RCC,EXTI,NVIC}/*.c Register-level driver implementations
```

## Architecture: layered MCAL style

The code follows a classic **AUTOSAR-flavored layered architecture**, split into three tiers (mirrored identically in `Target_Bootloader`):

| Layer                                        | Directory      | Responsibility                                                                                                                                                               |
| -------------------------------------------- | -------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Library**                                  | `Library/`     | Toolchain-, chip-, and core-level plumbing: fixed-width types, bit macros, CMSIS core headers, startup code, linker script. No hardware behavior of its own.                 |
| **MCAL** (Microcontroller Abstraction Layer) | `MCAL/`        | One driver per on-chip peripheral (GPIO, RCC, EXTI, NVIC), each with a public `*_Interface.h` and a `*_Program.c` implementation. Talks directly to memory-mapped registers. |
| **Application**                              | `Application/` | The actual product logic — here, a trivial LED blink — built purely on top of the MCAL API.                                                                                  |

Every module follows the same two-file **Interface/Program** convention: the interface header declares opaque types (enums/structs) and a documented function-prototype API; the program file implements it against raw registers. This keeps `Application.c` hardware-agnostic — it never touches a register directly, only `MCAL_GPIO_*` calls.

## Component details

### Library

- **`Standard_Types.h`** — `u8/u16/u32/u64`, `s8/s16/s32/s64`, `f32/f64/f128` — the fixed-width aliases used everywhere instead of raw C types.
- **`Macros.h`** — `logic_t` (`Enable/Disable`, `True/False`, `TRUE/FALSE` all as one enum), bit-manipulation macros (`Set_Bit`, `Clear_Bit`, `Toggle_Bit`, `Get_Bit(s)`, `Write_Bits`), and a busy-wait `delay_ms()`.
- **`MCU/STM32F103.h`** — peripheral base addresses (`ROM_BASE 0x08000000`, `RAM_BASE 0x20000000`, `Pheriphals_BASE 0x40000000`, ...), bus offsets for every peripheral (GPIOx, USARTx, RCC, CRC, FLASH, ...), and a `Peripheral_BitBand()` macro for the Cortex-M3 bit-banding region.
- **`Cortex_M3/Headers/`** — vendor CMSIS files (`core_cm3.h`, `stm32f1xx.h`, `system_stm32f1xx.h`, `cmsis_gcc.h`) providing the `RCC`, `GPIOx`, `SCB` struct definitions and intrinsics used by both the Library and MCAL layers.
- **`Cortex_M3/Startup.s`** — the reset/exception vector table (`g_pfnVectors`) and `Reset_Handler`: copies `.data` from flash to RAM, zeroes `.bss`, then branches to `main`. Every STM32F1 IRQ (`WWDG`, `EXTI0..15`, `USARTx`, `DMA1_Channelx`, ...) is declared `weak` and aliased to `Default_Handler` unless overridden.
- **`Cortex_M3/System_Init.c`** — unmodified ST CMSIS `SystemInit()` / `SystemCoreClockUpdate()`: resets `RCC` to its power-on state and relocates the vector table to `FLASH_BASE + VECT_TAB_OFFSET (0x8000)` — i.e. it already assumes code lives _after_ a 32 KB bootloader region (see Memory Map below).
- **`Linker_Script.ld`** — STM32F103C8Tx memory map.

### MCAL

|Module|File pair|API surface|
|---|---|---|
|**GPIO**|`GPIO_Interface.h` / `GPIO_Program.c`|`MCAL_GPIO_Initialize` (mode/speed per pin via `GPIO_Pin_Config_t`), `MCAL_GPIO_Write_Pin`, `MCAL_GPIO_Read_Pin`, `MCAL_GPIO_Toggle_Pin`, whole-port `Write_Port`/`Read_Port`/`Toggle_Port`, and `MCAL_GPIO_Lock_Pin` (STM32 `LCKR` lock sequence). Supports ports A–D, 16 pins each, all STM32F1 modes (analog, floating/pull-up/pull-down input, push-pull/open-drain output, AF variants) at 2/10/50 MHz.|
|**RCC**|`RCC_Interface.h` / `RCC_Program.c`|`RCC_Clock(module, Enable\|Disable)` gates the clock for any AHB/APB1/APB2 peripheral (DMA, GPIOx, USARTx, SPIx, I2Cx, CRC, ...) via a single enum, `RCC_Get_*_Clock()` accessors for system/AHB/APB1/APB2 frequency.|
|**EXTI**|`EXTI_Interface.h` / `EXTI_Program.c`|`EXTI_Initilization(EXTI_Config_t)` wires a GPIO pin to an external interrupt line (rising/falling/both edge sensing) and registers a callback pointer; `EXTI_Pins_Victor_Map` + `EXTI_IRQHandler(...)` macro map pins 0–15 to their NVIC vectors.|
|**NVIC**|`NVIC_Interface.h` / `NVIC_Program.c`|`NVIC_Interrupt(irq, Enable\|Disable)` — single call to arm/disarm any Cortex-M3/STM32F1 IRQ, with the full STM32F103 IRQ-number table as an enum.|

### Application

- **`Application.h`** — build/version config (`CHIP_ID`, `MAJOR`, `MINOR`, `DELAY`).
- **`Application.c`**:
    - `System_Initialization()` — configures `GPIO_B` pins 0, 1, 14, 15 as push-pull outputs and drives them low (LEDs off).
    - `Test_Blue()` / `Test_Yellow()` — toggle pin pairs (0/1 and 14/15) with a `delay_ms(DELAY)` between edges — a simple two-color blink demo.
    - `main()` — calls `System_Initialization()` then spins forever; the two `Test_*` calls are currently commented out (the blink itself is disabled — the binary mainly serves as a bootloader-jump target with LEDs held low).

## Memory map

Two things about placement matter for the OTA story:

- **`Linker_Script.ld`** places `.isr_vector`/`.text`/`.rodata` at `FLASH ORIGIN = 0x08008000, LENGTH = 32K` — i.e. **32 KB into the chip's 64 KB flash**, leaving the first 32 KB for [`Target_Bootloader`](https://vscode-remote+ssh-002dremote-002b7b22686f73744e616d65223a225043227d.vscode-resource.vscode-cdn.net/home/khaled/Documents/Github/FOTA/Target_Bootloader). RAM is `0x20000000`, 20 KB, stack top `_estack = 0x20005000`.
- **`System_Init.c`** relocates `SCB->VTOR` to `FLASH_BASE + 0x8000` for the same reason — the vector table CPU jumps to on reset is the _bootloader's_, and only after the bootloader validates/jumps does this application's own vector table become active.

> **Note:** `CMakeLists.txt`'s `flash` target writes the raw `.bin` to `FLASH_Address = 0x08000000` (chip start), not `0x08008000`. That target is a convenience for standalone bring-up/debugging of this application by itself; the intended production path is via the bootloader's `Flash_Application` command, which writes to the offset the linker script actually expects.

## Build & flash

```bash
mkdir build && cd build
cmake ..
make                       # -> Test.<ChipID>.<Major>.<Minor>.{bin,hex,axf,map}
make flash                 # st-flash write <bin> 0x08000000  (standalone bring-up)
make erase                 # st-flash erase
make read                  # dump flash back out for inspection
```

Toolchain: `arm-none-eabi-gcc/g++/as`, Cortex-M3, `-mthumb -mcpu=cortex-m3`, C11/C++17, linked with `--specs=nano.specs --specs=nosys.specs` (no OS, minimal libc). CMake version-tags the output binary as `Test.<Chip_ID>.<Major>.<Minor>` from variables set at the top of `CMakeLists.txt`.

The VS Code task (`.vscode/tasks.json`) runs the same `cmake && make`, then copies the resulting `.bin` into `../Build/` — the location the Raspberry Pi host (over NFS or via the GitHub-watched OTA flow described in the [root README](https://vscode-remote+ssh-002dremote-002b7b22686f73744e616d65223a225043227d.vscode-resource.vscode-cdn.net/home/khaled/Documents/Github/FOTA/README.md)) picks up as the new firmware image to flash through the bootloader.

## Relationship to the rest of the repo

- **Target_Bootloader**  shares this project's `Library`/MCAL base almost verbatim (same `Startup.s`, near-identical GPIO/RCC drivers), and adds CRC/FLASH/USART drivers plus the bootloader protocol/state machine. It occupies flash `0x08000000`–`0x08007FFF` and is what decides whether to jump into this application.
- **CPP_Application/Bootloader)** — the Raspberry Pi–side C++ host that talks to `Target_Bootloader` over UART and pushes this application's compiled `.bin` as the OTA payload.