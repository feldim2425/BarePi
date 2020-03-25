
set(
    SOURCES
    "start.S"
    "start.c"
    #"semihost.c"
)

add_library(BarePI OBJECT ${SOURCES})
target_include_directories(BarePI PUBLIC "../includes")