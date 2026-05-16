#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include <stdint.h>
#include <math.h>
#include "han_shu.h"
#include "cmsis_os2.h"
#include "stdio.h"
#include <stdarg.h>

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
