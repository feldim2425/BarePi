
set(
    SOURCES
    "start/start.S"
    "start.c"
    "semihost.c"
)

add_compile_options("-Wall")

add_library(BarePI OBJECT ${SOURCES})
target_include_directories(BarePI PUBLIC "../includes")