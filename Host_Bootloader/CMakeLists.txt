#Building Message
message(STATUS "================== Building Application ==================")
#Set Version
cmake_minimum_required(VERSION 3.0)
#Set Project Name   
project(Bootloader VERSION 22.22)
#Locate Source Files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR} "Source/*.c" "Source/*.cpp")
#Building Message
message(STATUS "Files To Be Compiled : ${SOURCES}")
# Specify include directories relative to the current CMake file
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
#Add Executable
add_executable(${PROJECT_NAME} ${SOURCES})
#CMake based applications using the SDK
set(CMAKE_TOOLCHAIN_FILE $ENV{OE_CMAKE_TOOLCHAIN_FILE})
#Install executable to binary directory
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
#Building Message
message(STATUS "================ Application Build Finish ================")

