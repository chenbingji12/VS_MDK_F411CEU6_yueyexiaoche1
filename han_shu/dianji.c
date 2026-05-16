#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include <stdint.h>
#include <math.h>
#include "han_shu.h"
#include "cmsis_os2.h"
#include "stdio.h"
#include <stdarg.h>

extern TIM_HandleTypeDef htim3;

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
