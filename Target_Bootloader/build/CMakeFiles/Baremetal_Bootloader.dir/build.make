# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build

# Include any dependencies generated for this target.
include CMakeFiles/Baremetal_Bootloader.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Baremetal_Bootloader.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Baremetal_Bootloader.dir/flags.make

CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj: ../Source/Application/Baremetal_Bootloader.c
CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Application/Baremetal_Bootloader.c

CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Application/Baremetal_Bootloader.c > CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Application/Baremetal_Bootloader.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj: ../Source/Library/Bootloader/Bootloader_Program.c
CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Bootloader/Bootloader_Program.c

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Bootloader/Bootloader_Program.c > CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Bootloader/Bootloader_Program.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.obj: ../Source/Library/Cortex_M3/Startup.s
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building ASM object CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.obj"
	/usr/bin/arm-none-eabi-as $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Cortex_M3/Startup.s

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing ASM source to CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.i"
	/usr/bin/arm-none-eabi-as $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Cortex_M3/Startup.s > CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.i

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling ASM source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.s"
	/usr/bin/arm-none-eabi-as $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Cortex_M3/Startup.s -o CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.s

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj: ../Source/Library/Cortex_M3/System_Init.c
CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Cortex_M3/System_Init.c

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Cortex_M3/System_Init.c > CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/Library/Cortex_M3/System_Init.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj: ../Source/MCAL/CRC/CRC.c
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/CRC/CRC.c

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/CRC/CRC.c > CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/CRC/CRC.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj: ../Source/MCAL/EXTI/EXTI_Program.c
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/EXTI/EXTI_Program.c

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/EXTI/EXTI_Program.c > CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/EXTI/EXTI_Program.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj: ../Source/MCAL/FLASH/FLASH.c
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/FLASH/FLASH.c

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/FLASH/FLASH.c > CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/FLASH/FLASH.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj: ../Source/MCAL/GPIO/GPIO_Program.c
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/GPIO/GPIO_Program.c

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/GPIO/GPIO_Program.c > CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/GPIO/GPIO_Program.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj: ../Source/MCAL/NVIC/NVIC_Program.c
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/NVIC/NVIC_Program.c

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/NVIC/NVIC_Program.c > CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/NVIC/NVIC_Program.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj: ../Source/MCAL/RCC/RCC_Program.c
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/RCC/RCC_Program.c

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/RCC/RCC_Program.c > CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/RCC/RCC_Program.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.s

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/flags.make
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj: ../Source/MCAL/USART/USART_Program.c
CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj: CMakeFiles/Baremetal_Bootloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj -MF CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj.d -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj -c /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/USART/USART_Program.c

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.i"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/USART/USART_Program.c > CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.i

CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.s"
	/usr/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/Source/MCAL/USART/USART_Program.c -o CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.s

# Object files for target Baremetal_Bootloader
Baremetal_Bootloader_OBJECTS = \
"CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj" \
"CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj"

# External object files for target Baremetal_Bootloader
Baremetal_Bootloader_EXTERNAL_OBJECTS =

Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/Application/Baremetal_Bootloader.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Bootloader/Bootloader_Program.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/Startup.s.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/Library/Cortex_M3/System_Init.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/CRC/CRC.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/EXTI/EXTI_Program.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/FLASH/FLASH.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/GPIO/GPIO_Program.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/NVIC/NVIC_Program.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/RCC/RCC_Program.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/Source/MCAL/USART/USART_Program.c.obj
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/build.make
Baremetal_Bootloader: CMakeFiles/Baremetal_Bootloader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking C executable Baremetal_Bootloader"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Baremetal_Bootloader.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/arm-none-eabi-objcopy -O ihex Baremetal_Bootloader Baremetal_Bootloader.hex
	/usr/bin/cmake -E copy Baremetal_Bootloader Baremetal_Bootloader.axf
	/usr/bin/arm-none-eabi-objcopy -O binary Baremetal_Bootloader Baremetal_Bootloader.bin

# Rule to build all files generated by this target.
CMakeFiles/Baremetal_Bootloader.dir/build: Baremetal_Bootloader
.PHONY : CMakeFiles/Baremetal_Bootloader.dir/build

CMakeFiles/Baremetal_Bootloader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Baremetal_Bootloader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Baremetal_Bootloader.dir/clean

CMakeFiles/Baremetal_Bootloader.dir/depend:
	cd /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build /home/t0ti20/Desktop/VS-Code/GitHub/FOTA/Target_Bootloader/build/CMakeFiles/Baremetal_Bootloader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Baremetal_Bootloader.dir/depend

