#Set minimum required version if CMake
cmake_minimum_required(VERSION 3.12)

#include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

#set the name of the project (as PROJECT_NAME) and C/C++ standarts
project(Timer_Exercise3 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Creates a pico-sdk subdirectory in our project for the libreries 
pico_sdk_init()

#Tell CMake where to find the executable source file 
add_executable(${PROJECT_NAME}
    main.c
)

# Create C header file with the name <pio program>.pio.h>
#pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
) 

#enable USB output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
