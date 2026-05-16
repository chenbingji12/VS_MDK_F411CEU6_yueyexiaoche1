#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include <stdint.h>
#include <math.h>
#include "han_shu.h"
#include "cmsis_os2.h"
#include "stdio.h"
#include <stdarg.h>

extern TIM_HandleTypeDef htim11;

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
