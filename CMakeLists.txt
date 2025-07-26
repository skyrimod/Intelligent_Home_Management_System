cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME Intelligent_Home_Management_System)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

set(BSP_Src
        BSP/adapter/src/sensor_manager.c
        BSP/drv/src/lcd.c
        BSP/drv/src/dht11.c
        BSP/drv/src/dwt_delay.c
        BSP/drv/src/mq2.c
        BSP/drv/src/sht30.c
)

set(App_Tasks
        Application/Tasks/Src/tasks_manager.c
        Application/Tasks/Src/init_task.c
        Application/Tasks/Src/lcd_task.c
        Application/Tasks/Src/log_task.c
        Application/Tasks/Src/wdg_task.c
        Application/Tasks/Src/sensor_task.c
)

set(EasyLogger_Src
        Middlewares/EasyLogger/src/elog.c
        Middlewares/EasyLogger/src/elog_utils.c
        Middlewares/EasyLogger/port/elog_port.c
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
        ${EasyLogger_Src}
        ${BSP_Src}
        ${App_Tasks}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
        BSP/drv/inc
        BSP/adapter/inc
        Application/Tasks/Inc
        Middlewares/EasyLogger/inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
