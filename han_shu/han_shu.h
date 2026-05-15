#ifndef han_shu_H
#define han_shu_H

void RightDianji(int right_speed);
void LeftDianji(int left_speed);
void Dianjisudu(int left_speed,int right_speed);
int huiduduqu(void);
int huiduzhi(int tongdao);
uint32_t zonghuiduzhi(void);
void TIM11_Delay_us(uint16_t us);
float chaoshenboceju(void);

typedef struct {
    int16_t raw_accel_x, raw_accel_y, raw_accel_z;  // 原始ADC值
    int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;     // 原始陀螺仪值
    float accel_x, accel_y, accel_z;  // 加速度 (g)
    float gyro_x, gyro_y, gyro_z;     // 角速度 (°/s)
} MPU6050_Data;

// 函数声明
uint8_t MPU6050_ReadFiltered(MPU6050_Data* data);
float Get_Z_RotationAngle(void);
void Xuanzhuan(float target);
void Zhizou(void);

#endif
