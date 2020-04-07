cmake_minimum_required(VERSION 3.3)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(NOT RASPI_VERSION)
    set(RASPI_VERSION 2)
    message(STATUS "No RASPI_VERSION specified, using default: ${RASPI_VERSION}")
else()
    message(STATUS "Using RASPI_VERSION: ${RASPI_VERSION}")
endif()

set(ARM_TOOLCHAIN_BITNESS_ALLOWED 32 64)
if(NOT ARM_TOOLCHAIN_BITNESS)
    set(AARCH64_RASPI_VERSIONS 3 4)
    if(RASPI_VERSION IN_LIST AARCH64_RASPI_VERSIONS)
        set(ARM_TOOLCHAIN_BITNESS 64)
    else()
        set(ARM_TOOLCHAIN_BITNESS 32)
    endif()
elseif(NOT ARM_TOOLCHAIN_BITNESS IN_LIST ARM_TOOLCHAIN_BITNESS_ALLOWED)
    message(FATAL_ERROR "ARM_TOOLCHAIN_BITNESS ${ARM_TOOLCHAIN_BITNESS} is not supported!")
endif()
message(STATUS "Using Toolchain bitness: ${ARM_TOOLCHAIN_BITNESS}")

if(NOT ARM_TOOLCHAIN_TRIPLET)
    if(ARM_TOOLCHAIN_BITNESS EQUAL 64)
        set(ARM_TOOLCHAIN_TRIPLET "aarch64-none-elf")
    else()
        set(ARM_TOOLCHAIN_TRIPLET "arm-none-eabi")
    endif()
    message(STATUS "No ARM_TOOLCHAIN_TRIPLET specified, using default: ${ARM_TOOLCHAIN_TRIPLET}")
endif()

set(CMAKE_C_COMPILER "${ARM_TOOLCHAIN_TRIPLET}-gcc")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")
set(CMAKE_CXX_COMPILER "${ARM_TOOLCHAIN_TRIPLET}-g++")
set(CMAKE_OBJCOPY "${ARM_TOOLCHAIN_TRIPLET}-objcopy")
set(CMAKE_OBJDUMP "${ARM_TOOLCHAIN_TRIPLET}-objdump")
set(CMAKE_SIZE    "${ARM_TOOLCHAIN_TRIPLET}-size")
set(CMAKE_CPPFILT "${ARM_TOOLCHAIN_TRIPLET}-c++filt")

set(ARM_LINKER_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../linker")

if(RASPI_VERSION EQUAL 1)
    set(ARM_PLATFORM_FLAGS "-mcpu=arm1176jz-s -mfpu=vfp")
    set(ARM_LINKER_SCRIPT "${ARM_LINKER_DIRECTORY}/raspi32.ld")
elseif(RASPI_VERSION EQUAL 2)
    set(ARM_PLATFORM_FLAGS "-mcpu=cortex-a7 -mfpu=neon-vfpv4")
    set(ARM_LINKER_SCRIPT "${ARM_LINKER_DIRECTORY}/raspi32.ld")
elseif(RASPI_VERSION EQUAL 3)
    set(ARM_PLATFORM_FLAGS "-mcpu=cortex-a53 -mfpu=neon-fp-armv8")
    set(ARM64_PLATFORM_FLAGS "-mcpu=cortex-a53")
    set(ARM_LINKER_SCRIPT "${ARM_LINKER_DIRECTORY}/raspi64.ld")
elseif(RASPI_VERSION EQUAL 4)
    set(ARM_PLATFORM_FLAGS "-mcpu=cortex-a72 -mfpu=neon-fp-armv8 ")
    set(ARM64_PLATFORM_FLAGS "-mcpu=cortex-a72 ")
    set(ARM_LINKER_SCRIPT "${ARM_LINKER_DIRECTORY}/raspi64.ld")
else()
    message(FATAL "Unknown Raspberry version")
endif()

if(ARM_TOOLCHAIN_BITNESS EQUAL 64)
    set(ARM_TOOLCHAIN_FLAGS "${ARM64_PLATFORM_FLAGS} -g -ffreestanding -nostartfiles")
    add_compile_definitions(AARCH=64)
else()
    set(ARM_TOOLCHAIN_FLAGS "${ARM_PLATFORM_FLAGS} -g -marm -ffreestanding -nostartfiles")
endif()

add_compile_definitions(PIBOARD=${RASPI_VERSION})

set(CMAKE_C_FLAGS "${ARM_TOOLCHAIN_FLAGS}")
set(CMAKE_CXX_FLAGS "${ARM_TOOLCHAIN_FLAGS}")
set(CMAKE_ASM_FLAGS "${ARM_TOOLCHAIN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${ARM_TOOLCHAIN_FLAGS} -T ${ARM_LINKER_SCRIPT} -L ${ARM_LINKER_DIRECTORY}")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

function(rpi_add_executable TARGET)
    add_executable(${TARGET} ${ARGN})
    set_target_properties(${TARGET} PROPERTIES LINK_DEPENDS "${ARM_LINKER_SCRIPT};${ARM_LINKER_DIRECTORY}/barepi.ld")
    add_custom_target(${TARGET}.img ALL DEPENDS ${TARGET} COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${TARGET}> -O binary $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.img)
endfunction()
