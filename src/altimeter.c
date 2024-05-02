/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 * Modified 2024 by TechnoDot
 * nah i modified it too much its mine now
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "data.h"

// 7-bit address
#define MPL3115A2_ADDR 0x60
#define MPL3115A2_SDA 2
#define MPL3115A2_SCL 3

#define MPL3115A2_STATUSREG _u(0x00)
#define MPL3115A2_CTRLREG1 _u(0x26)
#define MPL3115A2_CTRLREG2 _u(0x27)
#define MPL3115A2_OUT_P_MSB _u(0x01)
#define MPL3115A2_OUT_P_CSB _u(0x02)
#define MPL3115A2_OUT_P_LSB _u(0x03)

void mpl3115a2_init() {
    gpio_set_function(MPL3115A2_SDA, GPIO_FUNC_I2C);
    gpio_set_function(MPL3115A2_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(MPL3115A2_SDA);
    gpio_pull_up(MPL3115A2_SCL);
    
    // set as altimeter with oversampling ratio of 128
    uint8_t buf[] = {MPL3115A2_CTRLREG1, 0xB8};
    i2c_write_blocking(i2c1, MPL3115A2_ADDR, buf, 2, false);

    // set data refresh every 2 seconds, 0 next bits as we're not using those interrupts
    buf[0] = MPL3115A2_CTRLREG2, buf[1] = 0x00;
    i2c_write_blocking(i2c1, MPL3115A2_ADDR, buf, 2, false);

    buf[0] = MPL3115A2_STATUSREG, buf[1] = 0x00;
    i2c_write_blocking(i2c1, MPL3115A2_ADDR, buf, 2, false);

    // set device active
    buf[0] = MPL3115A2_CTRLREG1, buf[1] = 0xB9;
    i2c_write_blocking(i2c1, MPL3115A2_ADDR, buf, 2, false);
}

void mpl3115a2_read(int32_t *altitude) {
    // I have next to no experience with C and the first thing i need to do is comb through datasheets and query the altimeter
    // I HAVE NO EXPERIENCE WITH C OR I2C OR ANYTHING HERE AAAA

    uint8_t reg;

    uint8_t p_msb;
    uint8_t p_csb;
    uint8_t p_lsb;
    
    reg = MPL3115A2_OUT_P_MSB;
    i2c_write_blocking(i2c1, MPL3115A2_ADDR, &reg, 1, true); // keep control of bus
    i2c_read_blocking(i2c1, MPL3115A2_ADDR, &p_msb, 1, false);
    
    reg = MPL3115A2_OUT_P_CSB;
    i2c_write_blocking(i2c1, MPL3115A2_ADDR, &reg, 1, true); // keep control of bus
    i2c_read_blocking(i2c1, MPL3115A2_ADDR, &p_csb, 1, false);

    reg = MPL3115A2_OUT_P_LSB;
    i2c_write_blocking(i2c1, MPL3115A2_ADDR, &reg, 1, true); // keep control of bus
    i2c_read_blocking(i2c1, MPL3115A2_ADDR, &p_lsb, 1, false);

    *altitude = (p_lsb >> 4) + p_csb * 16 + p_msb * 16 * 256;
}