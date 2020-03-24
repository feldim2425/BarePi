
list(
    APPEND
    SOURCES
    "start.c"
    "semihost.c"
)

add_library(BarePI STATIC ${SOURCES})
target_include_directories(BarePI PUBLIC "../includes")