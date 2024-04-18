#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// #include "accelerometer.c"
// #include "altimeter.c"

#include "data.h"

int led = 0;

void update_led() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led);
    if (led) {
        led = 0;
    } else {
        led = 1;
    }
}

int main() {
    stdio_init_all();
    
    printf("Booting missleOS v1.0...\n");

    if (cyw43_arch_init()) {
        printf("Wireless initialization failed\n");
        return -1;
    } else {
        printf("Wireless comms initialized [1/3]\n");
    }

    // mpu6050_init();
    // printf("MPU6050 accelerometer initialized [2/3]\n");

    // mpl3115a2_init();
    // printf("MPL3115A2 altimeter initialized [3/3]\n");

    printf("Hello World!\n");
    printf("missleOS v1.0 succesfully booted!\n");

    while (true) {
        update_led();
        sleep_ms(250);
    }
}
