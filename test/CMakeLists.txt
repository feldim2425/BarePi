set(
    SOURCES
    "main.c"
)

rpi_add_executable(BarePiTest ${SOURCES})
target_link_libraries(BarePiTest BarePI)
