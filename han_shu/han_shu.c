#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include <stdint.h>
#include <math.h>
#include "han_shu.h"
#include "cmsis_os2.h"
#include "stdio.h"
#include <stdarg.h>

extern void debug_printf(const char *fmt, ...);

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim11;
extern I2C_HandleTypeDef hi2c1;

// ========== I2C 中断模式同步 ==========
static osSemaphoreId_t i2c_sem = NULL;

static void i2c_sem_init(void) {
    if(!i2c_sem)
        i2c_sem = osSemaphoreNew(1, 0, NULL);
}

static HAL_StatusTypeDef i2c_wait(uint32_t timeout_ms) {
    if(osSemaphoreAcquire(i2c_sem, timeout_ms) == osOK)
        return HAL_OK;
    // 超时：复位 I2C 外设释放总线
    HAL_I2C_DeInit(&hi2c1);
    HAL_I2C_Init(&hi2c1);
    return HAL_TIMEOUT;
}

// I2C HAL 中断完成回调（从 ISR 释放信号量）
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if(hi2c->Instance == I2C1) osSemaphoreRelease(i2c_sem);
}
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if(hi2c->Instance == I2C1) osSemaphoreRelease(i2c_sem);
}
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    if(hi2c->Instance == I2C1) osSemaphoreRelease(i2c_sem);
}

void LeftDianji(int left_speed)
{
    if(left_speed>100)
    {
        left_speed=100;
    }
    if(left_speed<-100)
    {
        left_speed=-100;
    }
    if(left_speed>0)
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,left_speed);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
    }
    else if (left_speed<0) {
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,-left_speed);
    }
    else {
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
    }

}

void RightDianji(int right_speed)
{
    if(right_speed>100)
    {
        right_speed=100;
    }
    if(right_speed<-100)
    {
        right_speed=-100;
    }
    if(right_speed>0)
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_speed);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
    }
    else if (right_speed<0) {
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,-right_speed);
    }
    else {
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
    }

}

void Dianjisudu(int left_speed,int right_speed)
{
    LeftDianji(left_speed);
    RightDianji(right_speed);

}

#define a1_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)
#define a1_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)
#define a2_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define a2_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define a3_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)
#define a3_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)

int huiduzhi(int tongdao)
{
    
    switch (tongdao) {
        case 0:
            a1_0; a2_0; a3_0;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        case 1:
            a1_0; a2_0; a3_1;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        case 2:
            a1_0; a2_1; a3_0;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        case 3:
            a1_0; a2_1; a3_1;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        case 4:
            a1_1; a2_0; a3_0;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        case 5:
            a1_1; a2_0; a3_1;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        case 6:
            a1_1; a2_1; a3_0;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        case 7:
            a1_1; a2_1; a3_1;
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
        default:
            return -1;
    }
}

uint32_t zonghuiduzhi(void)
{
return(10000000*huiduzhi(0)+1000000*huiduzhi(1)+100000*huiduzhi(2)+10000*huiduzhi(3)+1000*huiduzhi(4)+100*huiduzhi(5)+10*huiduzhi(6)+huiduzhi(7));
}

void TIM11_Delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim11,0);
    while(__HAL_TIM_GET_COUNTER(&htim11)<us);
}

float chaoshenboceju(void)
{
    float distance;
    uint32_t timeout;
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    TIM11_Delay_us(30);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    
    // 重置计数器，用于超时判断
    __HAL_TIM_SET_COUNTER(&htim11, 0);
    
    // 等待Echo变高，超时5000us（约85cm）
    timeout = 5000;
    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==GPIO_PIN_RESET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim11) > timeout)
        {
            return 999.0f;  // 超时返回，表示无障碍物
        }
    }
    
    __HAL_TIM_SET_COUNTER(&htim11,0);
    
    // 等待Echo变低，超时25000us（约425cm）
    timeout = 25000;
    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==GPIO_PIN_SET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim11) > timeout)
        {
            return 999.0f;  // 超时返回
        }
    }
    
    distance=__HAL_TIM_GET_COUNTER(&htim11)*0.034/2;
    return distance;
}

// 全局变量
static float angle_z = 0.0f;
static uint32_t last_time = 0;
static uint8_t initialized = 0;

#define MPU6050_ADDR 0xD0  // 0x68 << 1
#define PWR_MGMT_1   0x6B
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43
#define WHO_AM_I     0x75

// 滤波器结构
static float accel_buffer[3][5] = {0};  // XYZ轴加速度滤波
static float gyro_buffer[3][5] = {0};   // XYZ轴陀螺仪滤波
static uint8_t buffer_idx = 0;

/**
  * @brief  函数1：读取并滤波MPU6050数据
  * @param  data: 存储数据的结构体
  * @retval 0:失败, 1:成功
  */
uint8_t MPU6050_ReadFiltered(MPU6050_Data* data)
{
    uint8_t buffer[14];
    HAL_StatusTypeDef i2c_ret;
    uint8_t wr;
    
    i2c_sem_init();
    
    // 1. 初始化（中断模式，带重试）
    if(!initialized)
    {
        uint8_t check = 0xFF;
        
        // WHO_AM_I 读取 — 重试3次
        uint8_t init_retry = 3;
        do {
            check = 0xFF;
            i2c_ret = HAL_I2C_Mem_Read_IT(&hi2c1, MPU6050_ADDR, WHO_AM_I, 1, &check, 1);
            if(i2c_ret == HAL_OK) i2c_ret = i2c_wait(100);
            debug_printf("[MPU] WHO_AM_I=0x%02X, I2C_ret=%d (retry left=%d)\r\n",
                   check, (int)i2c_ret, init_retry);
            if(i2c_ret == HAL_OK && check == 0x68) break;
            HAL_Delay(20);
        } while(--init_retry);
        if(i2c_ret != HAL_OK || check != 0x68) return 0;
        
        // 唤醒（中断模式，带重试）
        uint8_t write_data = 0x00;
        for(wr = 0; wr < 3; wr++) {
            i2c_ret = HAL_I2C_Mem_Write_IT(&hi2c1, MPU6050_ADDR, PWR_MGMT_1, 1, &write_data, 1);
            if(i2c_ret == HAL_OK) { i2c_ret = i2c_wait(100); if(i2c_ret == HAL_OK) break; }
            debug_printf("[MPU] PWR_MGMT_1 write fail: %d\r\n", (int)i2c_ret);
            HAL_Delay(10);
        }
        if(i2c_ret != HAL_OK) return 0;
        HAL_Delay(50);  // 等待唤醒稳定（MPU6050 需要约 30ms 启动内部时钟）
        
        // 陀螺仪 ±500°/s（中断模式，带重试）
        write_data = 0x08;  // ±500°/s
        for(wr = 0; wr < 3; wr++) {
            i2c_ret = HAL_I2C_Mem_Write_IT(&hi2c1, MPU6050_ADDR, GYRO_CONFIG, 1, &write_data, 1);
            if(i2c_ret == HAL_OK) { i2c_ret = i2c_wait(100); if(i2c_ret == HAL_OK) break; }
            debug_printf("[MPU] GYRO_CONFIG write fail: %d\r\n", (int)i2c_ret);
            HAL_Delay(10);
        }
        if(i2c_ret != HAL_OK) return 0;
        
        // 加速度计 ±2g（中断模式，带重试）
        write_data = 0x00;  // ±2g
        for(wr = 0; wr < 3; wr++) {
            i2c_ret = HAL_I2C_Mem_Write_IT(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG, 1, &write_data, 1);
            if(i2c_ret == HAL_OK) { i2c_ret = i2c_wait(100); if(i2c_ret == HAL_OK) break; }
            debug_printf("[MPU] ACCEL_CONFIG write fail: %d\r\n", (int)i2c_ret);
            HAL_Delay(10);
        }
        if(i2c_ret != HAL_OK) return 0;
        
        // 采样率 100Hz（中断模式，带重试）
        write_data = 99;  // 1kHz/(99+1)=100Hz
        for(wr = 0; wr < 3; wr++) {
            i2c_ret = HAL_I2C_Mem_Write_IT(&hi2c1, MPU6050_ADDR, SMPLRT_DIV, 1, &write_data, 1);
            if(i2c_ret == HAL_OK) { i2c_ret = i2c_wait(100); if(i2c_ret == HAL_OK) break; }
            debug_printf("[MPU] SMPLRT_DIV write fail: %d\r\n", (int)i2c_ret);
            HAL_Delay(10);
        }
        if(i2c_ret != HAL_OK) return 0;
        
        // 5Hz低通滤波（中断模式，带重试）
        write_data = 0x06;
        for(wr = 0; wr < 3; wr++) {
            i2c_ret = HAL_I2C_Mem_Write_IT(&hi2c1, MPU6050_ADDR, CONFIG, 1, &write_data, 1);
            if(i2c_ret == HAL_OK) { i2c_ret = i2c_wait(100); if(i2c_ret == HAL_OK) break; }
            debug_printf("[MPU] CONFIG write fail: %d\r\n", (int)i2c_ret);
            HAL_Delay(10);
        }
        if(i2c_ret != HAL_OK) return 0;
        
        initialized = 1;
        debug_printf("[MPU] Init OK, rate=100Hz, gyro=±500dps, accel=±2g\r\n");
    }
    
    // 2. 一次性读取所有14个字节（中断模式，带重试）
    uint8_t rd_retry = 3;
    do {
        i2c_ret = HAL_I2C_Mem_Read_IT(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H, 1, buffer, 14);
        if(i2c_ret == HAL_OK) { i2c_ret = i2c_wait(100); if(i2c_ret == HAL_OK) break; }
        debug_printf("[MPU] Data read fail: %d (retry left=%d)\r\n", (int)i2c_ret, rd_retry - 1);
        HAL_Delay(10);
    } while(--rd_retry);
    if(i2c_ret != HAL_OK) return 0;
    
        // 3. 解析原始数据
    int16_t raw_accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    int16_t raw_accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    int16_t raw_accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);
    int16_t raw_gyro_x  = (int16_t)((buffer[8] << 8)  | buffer[9]);
    int16_t raw_gyro_y  = (int16_t)((buffer[10] << 8) | buffer[11]);
    int16_t raw_gyro_z  = (int16_t)((buffer[12] << 8) | buffer[13]);
    
    // 保存原始ADC值到结构体
    data->raw_accel_x = raw_accel_x;
    data->raw_accel_y = raw_accel_y;
    data->raw_accel_z = raw_accel_z;
    data->raw_gyro_x  = raw_gyro_x;
    data->raw_gyro_y  = raw_gyro_y;
    data->raw_gyro_z  = raw_gyro_z;
    
    // 4. 转换为实际值
    float accel_x = raw_accel_x / 16384.0f;  // ±2g: 16384
    float accel_y = raw_accel_y / 16384.0f;
    float accel_z = raw_accel_z / 16384.0f;
    float gyro_x  = raw_gyro_x  / 65.5f;     // ±500°/s: 65.5
    float gyro_y  = raw_gyro_y  / 65.5f;
    float gyro_z  = raw_gyro_z  / 65.5f;
    
    // 5. 移动平均滤波（5点）
    accel_buffer[0][buffer_idx] = accel_x;
    accel_buffer[1][buffer_idx] = accel_y;
    accel_buffer[2][buffer_idx] = accel_z;
    gyro_buffer[0][buffer_idx] = gyro_x;
    gyro_buffer[1][buffer_idx] = gyro_y;
    gyro_buffer[2][buffer_idx] = gyro_z;
    
    buffer_idx = (buffer_idx + 1) % 5;
    
    // 计算滤波后的值
    for(int i = 0; i < 3; i++)
    {
        float accel_sum = 0, gyro_sum = 0;
        
        for(int j = 0; j < 5; j++)
        {
            accel_sum += accel_buffer[i][j];
            gyro_sum += gyro_buffer[i][j];
        }
        
        if(i == 0) {
            data->accel_x = accel_sum / 5.0f;
            data->gyro_x = gyro_sum / 5.0f;
        }
        else if(i == 1) {
            data->accel_y = accel_sum / 5.0f;
            data->gyro_y = gyro_sum / 5.0f;
        }
        else {
            data->accel_z = accel_sum / 5.0f;
            data->gyro_z = gyro_sum / 5.0f;
        }
    }
    
    return 1;
}

/**
  * @brief  函数2：获取绕Z轴旋转角度
  * @retval 角度（度）
  */
float Get_Z_RotationAngle(void)
{
    MPU6050_Data data;
    
    if(!MPU6050_ReadFiltered(&data))
    {
        return angle_z;  // 读取失败，返回上一次的角度
    }
    
    uint32_t now = HAL_GetTick();
    
    if(last_time == 0)
    {
        last_time = now;
        return 0.0f;
    }
    
    float dt = (now - last_time) / 1000.0f;  // 秒
    
    if(dt > 0.1f) dt = 0.01f;
    
    // === 零偏校准（后台累积，不阻塞）===
    static float gyro_z_bias = 0.0f;
    static float bias_sum = 0.0f;
    static uint16_t bias_count = 0;
    
    if(bias_count < 100)
    {
        bias_sum += data.gyro_z;
        bias_count++;
        if(bias_count >= 100)
        {
            gyro_z_bias = bias_sum / 100.0f;
        }
    }
    
    // === 互补滤波 ===
    // 1. 加速度计检测静止（模方≈1g）
    float accel_mag_sq = data.accel_x * data.accel_x +
                         data.accel_y * data.accel_y +
                         data.accel_z * data.accel_z;
    
    if(fabs(accel_mag_sq - 1.0f) < 0.1f)
    {
        // 跟踪零偏温漂
        gyro_z_bias = gyro_z_bias * 0.99f + data.gyro_z * 0.01f;
        // 静止时缓慢归零
        angle_z *= 0.999f;
    }
    
    // 2. 去偏后的陀螺仪积分
    float corrected_gyro_z = data.gyro_z - gyro_z_bias;
    angle_z += corrected_gyro_z * dt;
    
    // 角度归一化
    if(angle_z > 180.0f) angle_z -= 360.0f;
    if(angle_z < -180.0f) angle_z += 360.0f;
    
    last_time = now;
    return angle_z;
}

// 带校准的旋转控制
void Xuanzhuan(float target)
{
    
    // 1. 校准起始角度
    float start_angle = 0.0f;
    for(int i = 0; i < 10; i++)
    {
        start_angle += Get_Z_RotationAngle();
        osDelay(20);
    }
    start_angle /= 10.0f;
    
    
    // 2. 旋转参数
    float current = 0.0f;      // 当前
    float kp = 0.8f;           // 比例系数
    float ki = 0.01f;          // 积分系数
    float kd = 0.1f;           // 微分系数
    float integral = 0.0f;
    float last_error = 0.0f;
    
    // 3. 开始旋转控制
    while(1)
    {
        // 获取当前绝对角度
        float absolute_angle = Get_Z_RotationAngle();
        
        // 计算相对旋转角度
        current = absolute_angle - start_angle;
        
        // 角度归一化
        if(current > 180.0f) current -= 360.0f;
        if(current < -180.0f) current += 360.0f;
        
        // 计算误差
        float error = target - current;
        
        
        // 判断是否到达
        if(fabs(error) < 2.0f)
        {
            
            Dianjisudu(0, 0);
            break;
        }
        
        // PID计算
        integral += error;
        if(integral > 100.0f) integral = 100.0f;
        if(integral < -100.0f) integral = -100.0f;
        
        float derivative = error - last_error;
        float output = kp * error + ki * integral + kd * derivative;
        
        // 输出限幅
        if(output > 100.0f) output = 100.0f;
        if(output < -100.0f) output = -100.0f;
        
        // 控制电机
        if(error < 0)
        {
            Dianjisudu(-output, output);
        }
        else {
            Dianjisudu(output, -output);
        }
        
        
        last_error = error;
        osDelay(20);
    }
}

void Zhizou(void)
{
    // 1. 校准起始角度
    float start_angle = 0.0f;
    for(int i = 0; i < 10; i++)
    {
        start_angle += Get_Z_RotationAngle();
        osDelay(20);
    }
    start_angle /= 10.0f;

// 2. 旋转参数
    float target = 0.0f;      // 目标
    float current = 0.0f;      // 当前
    float kp = 0.8f;           // 比例系数
    
    int sudu=60;

    // 获取当前绝对角度
        float absolute_angle = Get_Z_RotationAngle();
        
        // 计算相对旋转角度
        current = absolute_angle - start_angle;
        
        // 角度归一化
        if(current > 180.0f) current -= 360.0f;
        if(current < -180.0f) current += 360.0f;
        
        // 计算误差
        float error = target - current;

        while (1) {
        absolute_angle = Get_Z_RotationAngle();
        current = absolute_angle - start_angle;
        if(current > 180.0f) current -= 360.0f;
        if(current < -180.0f) current += 360.0f;
        error = target - current;

        // error<0(偏右)→左减速右加速; error>0(偏左)→左加速右减速
        Dianjisudu(sudu+error*kp, sudu-error*kp);

                MPU6050_Data data;
        MPU6050_ReadFiltered(&data);
        int16_t raw_AY = data.raw_accel_y;

        if(raw_AY<2000)
        {
            osDelay(10);
            MPU6050_ReadFiltered(&data);
            raw_AY = data.raw_accel_y;
            if(raw_AY<2000)
            {
                osDelay(10);
                MPU6050_ReadFiltered(&data);
                raw_AY = data.raw_accel_y;
                if(raw_AY<2000)
                {
                    break;
                }
            }
        }
}
}
