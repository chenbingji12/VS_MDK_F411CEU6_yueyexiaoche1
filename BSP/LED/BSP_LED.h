#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx_hal.h"

typedef enum
{
LED0=0,
LED1=1,
LED2=2,
LED3=3,
LED4=4,
LED5=5,
LED6=6,
LED7=7
}LED_INDEX;
//定义指示灯驱动方式数据类型

typedef enum
{
LOW_LEVEL=0,    
HIGH_LEVEL=1    //LED_DRIVE_LOW_LEVEL:低电平点亮，LED_DRIVE_HIGH_LEVEL:高电平点亮
}LED_DRIVE;

//指示灯配置函数
extern void BSP_LED_Config(LED_INDEX num,GPIO_TypeDef* port,uint16_t Pin,LED_DRIVE level);
extern void BSP_LED_On(LED_INDEX num);
extern void BSP_LED_Off(LED_INDEX num);
extern void BSP_LED_Toggle(LED_INDEX num);

#endif
