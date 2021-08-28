add_library(PCD-lib INTERFACE)

target_sources(PCD-lib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/pcd_lib.c
)

target_link_libraries(PCD-lib INTERFACE
    pico_stdlib
    hardware_spi
)