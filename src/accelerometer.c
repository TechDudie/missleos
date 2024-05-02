/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 * Modified 2024 by TechnoDot
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "data.h"

#define MPU6050_ADDR 0x68
#define MPU6050_SDA 4
#define MPU6050_SCL 5

#define MPL3115A2_ACCELREG _u(0x3B)
#define MPL3115A2_GYROREG _u(0x43)
#define MPL3115A2_TEMPREG _u(0x41)

void mpu6050_init() {
    gpio_set_function(MPU6050_SDA, GPIO_FUNC_I2C);
    gpio_set_function(MPU6050_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(MPU6050_SDA);
    gpio_pull_up(MPU6050_SCL);

    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x6B, 0x00}; // 0x80 did not work, 0x00 works; https://github.com/raspberrypi/pico-examples/pull/319
    i2c_write_blocking(i2c0, MPU6050_ADDR, buf, 2, false);
}

void mpu6050_read(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];
    uint8_t reg;

    // Start reading acceleration registers from register 0x3B for 6 bytes
    reg = MPL3115A2_ACCELREG;
    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    reg = MPL3115A2_GYROREG;
    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    reg = MPL3115A2_TEMPREG;
    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}