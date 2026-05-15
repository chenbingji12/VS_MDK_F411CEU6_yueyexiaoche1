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
set(CMAKE_PROJECT_NAME VS_CMAKE_F411CEU6_yueyexiaoche)

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
add_subdirectory(cmake/user)
# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    BSP/LED/BSP_LED.c
    han_shu/han_shu.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    BSP/LED
    han_shu
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)


# Add STM32CubeProgrammer ST-LINK flash target
find_program(STM32_PROGRAMMER_CLI
    NAMES STM32_Programmer_CLI.exe STM32_Programmer_CLI
)

if(STM32_PROGRAMMER_CLI)
    add_custom_target(flash
        COMMAND ${STM32_PROGRAMMER_CLI}
            -c port=SWD mode=UR
            -w $<TARGET_FILE:${CMAKE_PROJECT_NAME}> 0x08000000
            -rst
        DEPENDS ${CMAKE_PROJECT_NAME}
        COMMENT "Flashing ${PROJECT_NAME} via STM32CubeProgrammer ST-LINK..."
        VERBATIM
    )
else()
    message(STATUS "STM32CubeProgrammer ST-LINK CLI not found. Install STM32CubeProgrammer and add STM32_Programmer_CLI.exe to PATH to enable flash target.")
endif()
