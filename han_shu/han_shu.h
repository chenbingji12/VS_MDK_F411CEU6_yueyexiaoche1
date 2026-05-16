#ifndef han_shu_H
#define han_shu_H

// 8路灰度传感器分布值: 编码方式 = 每位权重×10^i, 使数值直观显示位模式
// 传感器排列: 位7(左侧) ~ 位0(右侧), 1=检测到黑线
#define h_00000000 0// 00000000: 无传感器检测到黑线(全白)
#define h_00000001 1    //00000001:仅传感器0(最右侧)检测到黑线
#define h_00000010 10//00000010: 仅传感器1检测到黑线
#define h_00000011 11//00000011: 传感器0-1检测到黑线
#define h_00000100 100 //00000100: 仅传感器2检测到黑线
#define h_00000110 110 //00000110: 传感器1-2检测到黑线
#define h_00001000 1000//00001000: 仅传感器3检测到黑线
#define h_00001100 1100//00001100: 传感器2-3检测到黑线
#define h_00001111 1111//00001111: 传感器0-3(右侧全部)检测到黑线
#define h_00010000 10000 // 00010000: 仅传感器4检测到黑线
#define h_00100000 100000 // 00100000: 仅传感器5检测到黑线
#define h_00110000 110000 // 00110000: 传感器4-5检测到黑线 
#define h_01000000 1000000 // 01000000: 仅传感器6检测到黑线
#define h_01100000 1100000 // 01100000: 传感器5-6检测到黑线
#define h_10000000 10000000 // 10000000: 仅传感器7(最左侧)检测到黑线
#define h_11000000 11000000 // 11000000: 传感器6-7(左侧全部)检测到黑线
#define h_11110000 11110000 // 11110000: 传感器4-7(左侧全部)检测到黑线
#define h_11111000 11111000 // 11111000: 传感器3-7检测到黑线
#define h_11111100 11111100 // 11111100: 传感器2-7检测到黑线
#define h_11111110 11111110 // 11111110: 传感器1-7检测到黑线
#define h_11111111 11111111 // 11111111: 全部传感器检测到黑线(全黑)
#define h_00011111 11111//00011111: 传感器0-4检测到黑线

extern volatile int yuansudu;//电机基础转速(原始速度), 作为Dianjisudu()的速度基准值

/*小车状态机*/
typedef enum {
    STATE_XUNJI,  // 跟随白线
    STATE_BIZHANG,  // 避障
    STATE_JIANSUDAI,  // 减速带
    STATE_XIAPO,  // 下坡
    STATE_STOP  // 停止
} CARSTATE;
extern CARSTATE CarState;  // 初始状态为循迹

/* 循迹控制结构体, 用于存储电机速度控制相关参数*/
typedef struct
{
  uint32_t huidufenbu;
  void (*zhixinghanshu)(void);
} HUIDUFENBU;

void cmd_1(void);
void cmd_2(void);
void cmd_3(void);
void cmd_4(void);
void cmd_5(void);
void cmd_6(void);
void cmd_7(void);
void cmd_8(void);
void cmd_9(void);
void cmd_10(void);
void cmd_11(void);

void xunji(uint32_t HUIDU);
void shizilukou(void);

/*减速带函数*/
void jiansudai(void);

/*下坡函数*/
void xiapo(void);

/*电机控制函数*/
void RightDianji(int right_speed);
void LeftDianji(int left_speed);
void Dianjisudu(int left_speed,int right_speed);

/*灰度传感器函数*/
int huiduduqu(void);
int huiduzhi(int tongdao);
uint32_t zonghuiduzhi(void);

/*超声波*/
void TIM11_Delay_us(uint16_t us);
float chaoshenboceju(void);

/*陀螺仪*/
typedef struct {
    int16_t raw_accel_x, raw_accel_y, raw_accel_z;  // 原始ADC值
    int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;     // 原始陀螺仪值
    float accel_x, accel_y, accel_z;  // 加速度 (g)
    float gyro_x, gyro_y, gyro_z;     // 角速度 (°/s)
} MPU6050_Data;

uint8_t MPU6050_ReadFiltered(MPU6050_Data* data);
float Get_Z_RotationAngle(void);
void Xuanzhuan(float target);
void Zhizou(void);

#endif
