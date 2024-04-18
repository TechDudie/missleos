cmake_minimum_required(VERSION 3.16.3)
include(missleos.cmake)
project(missleos VERSION 1.0.0)

set(PICO_BOARD pico_w)
pico_sdk_init()

add_executable(missleos src/altimeter.c)

target_link_libraries(missleos
pico_stdlib
pico_cyw43_arch_none
hardware_i2c
hardware_spi
)

pico_add_extra_outputs(missleos)
pico_enable_stdio_usb(missleos 1)
pico_enable_stdio_uart(missleos 0)