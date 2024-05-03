#/*******************************************************************
# *  FILE DESCRIPTION
#-----------------------
# *  Author: Khaled El-Sayed @t0ti20
# *  File: ./Main.cpp
# *  Date: March 28, 2024
# *  Description: Genaric CMake File For STM32F103C8 
# *  (C) 2023 "@t0ti20". All rights reserved.
#*******************************************************************/
# Building Message
message(STATUS "================== Building Application ==================")

# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.0)

# Set Project Name   
set(Application_Name Baremetal_Bootloader)

# Set CPP Standard
set(CMAKE_CXX_STANDARD 17)

# Flash Memory
set(FLASH_Address 0x08000000)
set(FLASH_Size 131072)

# Set Compiler Base Name
set(CMAKE_CC arm-none-eabi-)

# Compiler Name
set(CMAKE_C_COMPILER ${CMAKE_CC}gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_CC}g++)
set(CMAKE_ASM_COMPILER ${CMAKE_CC}as)

# Target definition
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Perform compiler test with static library
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set Project Name
project(${Application_Name} C CXX ASM)

# Locate Source Files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.s)


# Building Message
message(STATUS "Files To Be Compiled : ${SOURCES}")

# Compiler and linker flags
set(CMAKE_C_FLAGS "-std=gnu99 -Og -g -gdwarf-4 -mthumb -g2 -fno-builtin -mcpu=cortex-m3 -Wall  -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize -mlong-calls")
set(CMAKE_CXX_FLAGS " -std=c++11 -Og -g -mthumb -g2 -fno-builtin -mcpu=cortex-m3 -Wall  -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize -mlong-calls")
set(CMAKE_ASM_FLAGS "-g -mthumb -g2 -mcpu=cortex-m3")

# Search for the linker script file
file(GLOB_RECURSE LINKER_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/*.ld)

# Check if the linker script file is found
if(NOT LINKER_SCRIPT_PATH)
    message(FATAL_ERROR "Linker script file not found!")
endif()

# Set linker flags to include standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections --specs=nano.specs --specs=nosys.specs -mthumb -g2 -mcpu=cortex-m0plus -mabi=aapcs -T${LINKER_SCRIPT_PATH} -Wl,-Map=${Application_Name}.map" CACHE INTERNAL "Linker options")

# Add Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Collect all header files recursively from the Include directory
file(GLOB_RECURSE INCLUDE_FILES "${CMAKE_SOURCE_DIR}/*.hpp" "${CMAKE_SOURCE_DIR}/*.h")

# Add each header file to the project
foreach(INCLUDE_FILE ${INCLUDE_FILES})
    get_filename_component(INCLUDE_DIR ${INCLUDE_FILE} DIRECTORY)
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" RELATIVE_INCLUDE_DIR ${INCLUDE_DIR})
    string(REPLACE "/" "\\" RELATIVE_INCLUDE_DIR_WINDOWS ${RELATIVE_INCLUDE_DIR})
    target_sources(${PROJECT_NAME} PRIVATE ${INCLUDE_FILE})
    target_include_directories(${PROJECT_NAME} PRIVATE ${RELATIVE_INCLUDE_DIR})
endforeach()

# Generate Hex file
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${Application_Name}.hex
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME} ${Application_Name}.axf
)

# Generate Binary file
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${Application_Name}.bin
)

# Flash on board
add_custom_target(flash 
    COMMAND st-flash write ${CMAKE_CURRENT_BINARY_DIR}/${Application_Name}.bin ${FLASH_Address}
    COMMENT "Flashing finished"
)

# Erase the flash
add_custom_target(erase
    COMMAND st-flash erase
    COMMENT "Erasing finished"
)

# Read the flash
add_custom_target(read
    COMMAND st-flash read ${CMAKE_CURRENT_BINARY_DIR}/FLASH.bin ${FLASH_Address} ${FLASH_Size}
    COMMENT "Reading finished"
)
