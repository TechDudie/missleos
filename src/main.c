#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/flash.h"
#include "hardware/i2c.h"
#include "tusb.h"

#include "accelerometer.c"
#include "altimeter.c"

#include "data.h"

int led = 0;
int status = -1;
int i = 0;

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

    while (!(tud_cdc_connected() && tud_cdc_available())) {
        sleep_ms(10);
    }

    status = 0;
    
    printf("booting missleOS v1.0...\n");
    printf("[1/5] stdio initialized\n");
    
    if (cyw43_arch_init()) {
        printf("[2/5] wireless initialization failed\n");
        return -1;
    } else {
        printf("[2/5] wireless module initialized\n");
    }

    i2c_init(i2c0, 400 * 1000);
    i2c_init(i2c1, 400 * 1000);
    printf("[3/5] i2c bus initialized\n");
    sleep_ms(50);

    mpu6050_init();
    printf("[4/5] MPU6050 accelerometer initialized\n");
    sleep_ms(50);

    mpl3115a2_init();
    printf("[5/5] MPL3115A2 altimeter initialized\n");
    sleep_ms(50);

    printf("hello world!\n");
    printf("missleOS v1.0 succesfully booted!\n");
    status = 1;

    int16_t acceleration[3] = {0, 0, 0}, gyro[3] = {0, 0, 0}, temp = 0;
    int32_t altitude = 0;

    printf("================================================================================");

    while (true) {
        update_led();

        mpu6050_read(acceleration, gyro, &temp);
        printf("Accel: X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Accel: (g) X = %fg, Y = %fg, Z = %fg\n", acceleration[0] / 16384.0, acceleration[1] / 16384.0, acceleration[2] / 16384.0);
        printf("Gyro: X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
        printf("Gyro: (°/s) X = %f°/s, Y = %f°/s, Z = %f°/s\n", gyro[0] / 133.0, gyro[1] / 133.0, gyro[2] / 133.0);
        printf("Temp (chip): = %f\n", (temp / 340.0) + 36.53);
        printf("================================================================================");

        sleep_ms(250);

        mpl3115a2_read(&altitude);

        printf("Altitude: %d\n", altitude);
        printf("Altitude: (m) %fm\n", altitude / 16.0);
        printf("================================================================================");

        sleep_ms(250);
    }
}
