#include "stm32f4xx_hal.h"
////#include "stm32f4xx_hal_gpio.h"
////#include "stm32f4xx_hal_tim.h"
////#include <stdint.h>
////#include <math.h>
#include "han_shu.h"
////#include "cmsis_os2.h"
////#include "stdio.h"
////#include <stdarg.h>

void cmd_1(void){Xuanzhuan(-90);}
void cmd_2(void){Xuanzhuan(90);}
void cmd_3(void){Dianjisudu(yuansudu, yuansudu+5);}
void cmd_4(void){Dianjisudu(yuansudu+5, yuansudu);}
void cmd_5(void){Dianjisudu(yuansudu+15, yuansudu);}
void cmd_6(void){Dianjisudu(yuansudu, yuansudu+15);}
void cmd_7(void){Dianjisudu(yuansudu-15, yuansudu+20);}
void cmd_8(void){Dianjisudu(yuansudu+20, yuansudu-15);}
void cmd_9(void){Dianjisudu(yuansudu-25, yuansudu+30);}
void cmd_10(void){Dianjisudu(yuansudu+30, yuansudu-25);}
void cmd_11(void){Dianjisudu(yuansudu,yuansudu);}

HUIDUFENBU cmd_table[] = {
  {h_00001111, cmd_1},{h_00011111, cmd_1},
  {h_11110000, cmd_2},{h_11111000, cmd_2},
  {h_00010000, cmd_3},
  {h_00001000, cmd_4},
  {h_00000100, cmd_5},{h_00001100, cmd_5},
  {h_00100000, cmd_6},{h_00110000, cmd_6},
  {h_01000000, cmd_7},{h_01100000, cmd_7},
  {h_00000010, cmd_8},{h_00000110, cmd_8},
  {h_10000000, cmd_9},{h_11000000, cmd_9},
  {h_00000011, cmd_10},{h_00000001, cmd_10}
};

uint16_t cmd_table_size = sizeof(cmd_table) / sizeof(cmd_table[0]);

void xunji(uint32_t HUIDU)
{
 for(uint16_t i=0;i<cmd_table_size;i++)
 {
   if(HUIDU==cmd_table[i].huidufenbu)
   {
     cmd_table[i].zhixinghanshu();
     return;
   }
 }
 cmd_11();
}

