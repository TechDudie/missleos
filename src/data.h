#ifndef _MPU6050_DATA_T
#define _MPU6050_DATA_T

struct mpu6050_data_t {
    int16_t acceleration[3];
    int16_t gyro[3];
    int16_t temp;
};

#endif

#ifndef MPL3115A2_DATA_T
#define MPL3115A2_DATA_T

struct mpl3115a2_data_t {
    float temperature;
    float altitude;
};

#endif