#include "BSP_LED.h"

typedef struct
{
  GPIO_TypeDef* port; //指示灯对应的GPIO端口
  uint16_t pin;       //指示灯对应的GPIO引脚
  LED_DRIVE level;    //指示灯的驱动方式
}LED_TypeDef;

//指示灯数量的宏定义，默认为8个
#define LEDn 8

//定义指示灯对象数组，用于存放每个指示灯的属性，初始值赋0
static LED_TypeDef Leds[LEDn] = {0};

void BSP_LED_Config(LED_INDEX num,GPIO_TypeDef* port,uint16_t Pin,LED_DRIVE level)
{
//配置灯的属性：端口号，引脚号，驱动方式
Leds[num].port = port;
Leds[num].pin = Pin;
Leds[num].level = level;
}

void BSP_LED_On(LED_INDEX num)
{
  if(Leds[num].level == LOW_LEVEL) //如果是低电平点亮
  {
    HAL_GPIO_WritePin(Leds[num].port, Leds[num].pin, GPIO_PIN_RESET); //输出低电平
  }
  else //如果是高电平点亮
  {
    HAL_GPIO_WritePin(Leds[num].port, Leds[num].pin, GPIO_PIN_SET); //输出高电平
  }
}

void BSP_LED_Off(LED_INDEX num)
{
  if(Leds[num].level == LOW_LEVEL) //如果是低电平点亮
  {
    HAL_GPIO_WritePin(Leds[num].port, Leds[num].pin, GPIO_PIN_SET); //输出高电平
  }
  else //如果是高电平点亮
  {
    HAL_GPIO_WritePin(Leds[num].port, Leds[num].pin, GPIO_PIN_RESET); //输出低电平
  }
}

void BSP_LED_Toggle(LED_INDEX num)
{
  HAL_GPIO_TogglePin(Leds[num].port, Leds[num].pin);
}
