
set(
    SOURCE
    "start.c"
    "semihost.c"
    "multicore.c"
)

if(ARM_TOOLCHAIN_BITNESS EQUAL 64)
    list(APPEND SOURCES "start/start64.S")
else()
    list(APPEND SOURCES "start/start.S")
endif()

add_compile_options("-Wall")
add_compile_options("-fvar-tracking")

add_library(BarePI OBJECT ${SOURCES})
target_include_directories(BarePI PUBLIC "../includes")