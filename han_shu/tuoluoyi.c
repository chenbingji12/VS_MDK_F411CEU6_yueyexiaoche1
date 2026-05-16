#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <math.h>
#include "han_shu.h"
#include "cmsis_os2.h"

extern I2C_HandleTypeDef hi2c1;

// 常量定义
#define MPU6050_ADDR         0xD0
#define MPU6050_REG_DATA     0x3B  // 加速度计开始地址
#define MPU6050_REG_PWR_MGMT 0x6B
#define MPU6050_REG_WHO_AM_I 0x75

// 灵敏度转换系数 (乘法比除法快)
static const float ACCEL_SCALE = 1.0f / 16384.0f; // ±2g
static const float GYRO_SCALE  = 1.0f / 65.5f;    // ±500°/s

// 互斥锁：防止 StartTuoluoyi 与 StartYundong 并发访问 I2C1
static osMutexId_t i2c_mutex = NULL;

// 全局变量
static float angle_z = 0.0f;
static uint32_t last_time = 0;
static float gyro_z_bias = 0.0f;

/**
  * @brief  MPU6050 高速初始化
  * @retval 1:成功, 0:失败
  */
static uint8_t MPU6050_Init_Fast(void) {
    uint8_t check;
    uint8_t data;

    if(!i2c_mutex) i2c_mutex = osMutexNew(NULL);
    osMutexAcquire(i2c_mutex, osWaitForever);

    // 1. 检查 WHO_AM_I
    if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_REG_WHO_AM_I, 1, &check, 1, 100) != HAL_OK) {
        osMutexRelease(i2c_mutex);
        return 0;
    }
    if (check != 0x68) {
        osMutexRelease(i2c_mutex);
        return 0;
    }

    // 2. 唤醒并设置时钟源 (PLL with Z axis gyro reference)
    data = 0x03;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_REG_PWR_MGMT, 1, &data, 1, 100);

    // 3. 配置陀螺仪 ±500°/s
    data = 0x08;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1B, 1, &data, 1, 100);

    // 4. 配置加速度计 ±2g
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1C, 1, &data, 1, 100);

    // 5. 设置采样率 200Hz (1kHz / (1 + 4))
    data = 0x04;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x19, 1, &data, 1, 100);

    // 6. 低通滤波配置 (DLPF = 42Hz, 延时更低)
    data = 0x03;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1A, 1, &data, 1, 100);

    osMutexRelease(i2c_mutex);
    return 1;
}

/**
  * @brief  读取MPU6050原始数据并转换为物理量（已优化速度）
  * @param  data: 存储数据的结构体
  * @retval 0:失败, 1:成功
  */
uint8_t MPU6050_ReadFiltered(MPU6050_Data* data) {
    uint8_t buf[14];

    if(!i2c_mutex) {
        // 首次调用：先初始化 MPU6050
        i2c_mutex = osMutexNew(NULL);
        if(!MPU6050_Init_Fast()) return 0;
    }

    osMutexAcquire(i2c_mutex, osWaitForever);

    // 使用阻塞模式读取全部14字节
    if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_REG_DATA, 1, buf, 14, 10) != HAL_OK) {
        osMutexRelease(i2c_mutex);
        return 0;
    }

    // 快速位移合并
    int16_t r_ax = (int16_t)(buf[0] << 8 | buf[1]);
    int16_t r_ay = (int16_t)(buf[2] << 8 | buf[3]);
    int16_t r_az = (int16_t)(buf[4] << 8 | buf[5]);
    int16_t r_gx = (int16_t)(buf[8] << 8 | buf[9]);
    int16_t r_gy = (int16_t)(buf[10] << 8 | buf[11]);
    int16_t r_gz = (int16_t)(buf[12] << 8 | buf[13]);

    // 保存原始ADC值（StartTuoluoyi 中使用了 raw_accel_x, raw_accel_y, raw_gyro_z）
    data->raw_accel_x = r_ax;
    data->raw_accel_y = r_ay;
    data->raw_accel_z = r_az;
    data->raw_gyro_x  = r_gx;
    data->raw_gyro_y  = r_gy;
    data->raw_gyro_z  = r_gz;

    // 直接转换物理量
    data->accel_x = r_ax * ACCEL_SCALE;
    data->accel_y = r_ay * ACCEL_SCALE;
    data->accel_z = r_az * ACCEL_SCALE;
    data->gyro_x  = r_gx * GYRO_SCALE;
    data->gyro_y  = r_gy * GYRO_SCALE;
    data->gyro_z  = r_gz * GYRO_SCALE;

    osMutexRelease(i2c_mutex);
    return 1;
}

/**
  * @brief  获取Z轴旋转角度（极简优化版）
  * @retval 角度（度）
  */
float Get_Z_RotationAngle(void) {
    MPU6050_Data data;
    if (!MPU6050_ReadFiltered(&data)) return angle_z;

    uint32_t now = HAL_GetTick();
    if (last_time == 0) {
        last_time = now;
        return 0.0f;
    }

    float dt = (now - last_time) * 0.001f;
    last_time = now;

    // 零偏自动初始化（前100次读取）
    static uint16_t bias_cnt = 0;
    static float bias_sum = 0;
    if (bias_cnt < 100) {
        bias_sum += data.gyro_z;
        bias_cnt++;
        if(bias_cnt == 100) gyro_z_bias = bias_sum * 0.01f;
        return 0.0f;
    }

    // 积分运算
    float gz = data.gyro_z - gyro_z_bias;

    // 简单的死区处理：抑制静止时的漂移
    if (gz > -0.1f && gz < 0.1f) gz = 0;

    angle_z += gz * dt;

    // 归一化
    if (angle_z > 180.0f) angle_z -= 360.0f;
    else if (angle_z < -180.0f) angle_z += 360.0f;

    return angle_z;
}

// ================== 运动控制部分 ==================

void Xuanzhuan(float target) {
    float start_angle = Get_Z_RotationAngle();
    float kp = 1.2f, kd = 0.2f;
    float last_err = 0;

    while (1) {
        float current = Get_Z_RotationAngle() - start_angle;
        if (current > 180.0f) current -= 360.0f;
        if (current < -180.0f) current += 360.0f;

        float error = target - current;
        if (fabsf(error) < 1.0f) {
            Dianjisudu(0, 0);
            break;
        }

        float output = kp * error + kd * (error - last_err);
        last_err = error;

        if (output > 80.0f) output = 80.0f;
        if (output < -80.0f) output = -80.0f;

        Dianjisudu(output, -output);
        osDelay(10);
    }
}

void Zhizou(void) {
    float start_angle = Get_Z_RotationAngle();
    float kp = 2.5f;
    int base_speed = 60;

    while (1) {
        float current = Get_Z_RotationAngle() - start_angle;
        if (current > 180.0f) current -= 360.0f;
        if (current < -180.0f) current += 360.0f;

        float error = 0 - current;
        Dianjisudu(base_speed + error * kp, base_speed - error * kp);

        MPU6050_Data data;
        MPU6050_ReadFiltered(&data);
        if (data.raw_accel_y < 2000) {
            Dianjisudu(0,0);
            break;
        }
        osDelay(10);
    }
}
