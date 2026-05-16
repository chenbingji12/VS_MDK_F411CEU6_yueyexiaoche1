/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "cmsis_os2.h"
#include "stm32f4xx_hal_tim.h"
#include "han_shu.h"
#include "stm32f4xx_hal_uart.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//#include <sys/_intsup.h>

//cmake --build build/Debug --target flash

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* Definitions for Chaoshenbo */
osThreadId_t ChaoshenboHandle;
const osThreadAttr_t Chaoshenbo_attributes = {
  .name = "Chaoshenbo",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal6,
};
/* Definitions for Huidu */
osThreadId_t HuiduHandle;
const osThreadAttr_t Huidu_attributes = {
  .name = "Huidu",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal5,
};
/* Definitions for Tuoluoyi */
osThreadId_t TuoluoyiHandle;
const osThreadAttr_t Tuoluoyi_attributes = {
  .name = "Tuoluoyi",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for Yundong */
osThreadId_t YundongHandle;
const osThreadAttr_t Yundong_attributes = {
  .name = "Yundong",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for Lanyatouchuan */
osThreadId_t LanyatouchuanHandle;
const osThreadAttr_t Lanyatouchuan_attributes = {
  .name = "Lanyatouchuan",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for Yaokong */
osThreadId_t YaokongHandle;
const osThreadAttr_t Yaokong_attributes = {
  .name = "Yaokong",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityRealtime4,
};
/* Definitions for Qiehuan */
osThreadId_t QiehuanHandle;
const osThreadAttr_t Qiehuan_attributes = {
  .name = "Qiehuan",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal3,
};
/* Definitions for Jieshou */
osThreadId_t JieshouHandle;
const osThreadAttr_t Jieshou_attributes = {
  .name = "Jieshou",
  .stack_size = 300 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal6,
};
/* Definitions for Zonghuiduzhi */
osMessageQueueId_t ZonghuiduzhiHandle;
const osMessageQueueAttr_t Zonghuiduzhi_attributes = {
  .name = "Zonghuiduzhi"
};
/* Definitions for AX */
osMessageQueueId_t AXHandle;
const osMessageQueueAttr_t AX_attributes = {
  .name = "AX"
};
/* Definitions for AY */
osMessageQueueId_t AYHandle;
const osMessageQueueAttr_t AY_attributes = {
  .name = "AY"
};
/* Definitions for GZ */
osMessageQueueId_t GZHandle;
const osMessageQueueAttr_t GZ_attributes = {
  .name = "GZ"
};
/* Definitions for H1 */
osMessageQueueId_t H1Handle;
const osMessageQueueAttr_t H1_attributes = {
  .name = "H1"
};
/* Definitions for H2 */
osMessageQueueId_t H2Handle;
const osMessageQueueAttr_t H2_attributes = {
  .name = "H2"
};
/* Definitions for H3 */
osMessageQueueId_t H3Handle;
const osMessageQueueAttr_t H3_attributes = {
  .name = "H3"
};
/* Definitions for H4 */
osMessageQueueId_t H4Handle;
const osMessageQueueAttr_t H4_attributes = {
  .name = "H4"
};
/* Definitions for H5 */
osMessageQueueId_t H5Handle;
const osMessageQueueAttr_t H5_attributes = {
  .name = "H5"
};
/* Definitions for H6 */
osMessageQueueId_t H6Handle;
const osMessageQueueAttr_t H6_attributes = {
  .name = "H6"
};
/* Definitions for H7 */
osMessageQueueId_t H7Handle;
const osMessageQueueAttr_t H7_attributes = {
  .name = "H7"
};
/* Definitions for H8 */
osMessageQueueId_t H8Handle;
const osMessageQueueAttr_t H8_attributes = {
  .name = "H8"
};
/* Definitions for Shoujifasong */
osMutexId_t ShoujifasongHandle;
const osMutexAttr_t Shoujifasong_attributes = {
  .name = "Shoujifasong"
};
/* Definitions for Juli */
osSemaphoreId_t JuliHandle;
const osSemaphoreAttr_t Juli_attributes = {
  .name = "Juli"
};
/* USER CODE BEGIN PV */

volatile uint32_t zhi_2;   //全局变量，存储从消息队列中获取的值，以便在Yundong线程中使用

// --- DMA 非阻塞 printf 相关 ---
DMA_HandleTypeDef hdma_usart1_tx;               // USART1_TX DMA 句柄
static char debug_tx_buf[384];                   // 格式化缓冲区
static volatile uint8_t debug_tx_busy = 0;       // DMA 发送忙标志
volatile int16_t AX,AY,GZ;  //全局变量，存储加速度和陀螺仪原始值，以便在Tuoluoyi线程中使用
volatile int16_t ax,ay,gz;  //全局变量，存储从消息队列中获取的加速度和陀螺仪原始值，以便在Lanyatouchuan线程中使用
volatile uint8_t H1,H2,H3,H4,H5,H6,H7,H8;  //全局变量，存储8个灰度传感器通道的读取值，以便在Huidu线程中使用
volatile uint8_t h1,h2,h3,h4,h5,h6,h7,h8;  //全局变量，存储从消息队列中获取的8个灰度传感器通道的读取值，以便在Lanyatouchuan线程中使用
volatile uint8_t yaokong_flag=0;  //全局变量，存储遥控标志，以便在Yaokong线程中使用
volatile char Serial_RxPacket[20];  //全局变量，存储从串口接收的数据，以便在Yaokong线程中使用
volatile uint8_t RxFlag;  //全局变量，存储接收空闲中断标志
volatile uint8_t RxFlag_begin;  //全局变量，存储接收开始标志
volatile uint8_t RxCount=0;  //全局变量，存储接收数据的计数，以便在USART1_IRQHandler中使用

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM11_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
void StartChaoshenbo(void *argument);
void StartHuidu(void *argument);
void StartTuoluoyi(void *argument);
void StartYundong(void *argument);
void StartLanyatouchuan(void *argument);
void StartYaokong(void *argument);
void StartQiehuan(void *argument);
void StartJieshou(void *argument);

/* USER CODE BEGIN PFP */

/* DMA非阻塞调试输出函数前置声明 */
void debug_printf(const char *fmt, ...);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**typedef struct
{
  uint32_t huidufenbu;
  void (*zhixinghanshu)(void);
} HUIDUFENBU;

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
}**/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  // 调试模式下保持 TIM10/TIM11 继续计数
  __HAL_DBGMCU_FREEZE_TIM10();   // HAL 时钟基准（uwTick）
  __HAL_DBGMCU_FREEZE_TIM11();   // 超声波测距超时
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM11_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

HAL_TIM_Base_Start(&htim11);

__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
HAL_UART_Receive_DMA(&huart1, (uint8_t*)Serial_RxPacket, sizeof(Serial_RxPacket));
debug_printf("System Initialized\r\n");
  debug_printf("System \r\n");


  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of Shoujifasong */
  ShoujifasongHandle = osMutexNew(&Shoujifasong_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Juli */
  JuliHandle = osSemaphoreNew(0, 1, &Juli_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Zonghuiduzhi */
  ZonghuiduzhiHandle = osMessageQueueNew (1, sizeof(uint32_t), &Zonghuiduzhi_attributes);

  /* creation of AX */
  AXHandle = osMessageQueueNew (1, sizeof(int32_t), &AX_attributes);

  /* creation of AY */
  AYHandle = osMessageQueueNew (1, sizeof(int32_t), &AY_attributes);

  /* creation of GZ */
  GZHandle = osMessageQueueNew (1, sizeof(int32_t), &GZ_attributes);

  /* creation of H1 */
  H1Handle = osMessageQueueNew (1, sizeof(uint8_t), &H1_attributes);

  /* creation of H2 */
  H2Handle = osMessageQueueNew (1, sizeof(uint8_t), &H2_attributes);

  /* creation of H3 */
  H3Handle = osMessageQueueNew (1, sizeof(uint8_t), &H3_attributes);

  /* creation of H4 */
  H4Handle = osMessageQueueNew (1, sizeof(uint8_t), &H4_attributes);

  /* creation of H5 */
  H5Handle = osMessageQueueNew (1, sizeof(uint8_t), &H5_attributes);

  /* creation of H6 */
  H6Handle = osMessageQueueNew (1, sizeof(uint8_t), &H6_attributes);

  /* creation of H7 */
  H7Handle = osMessageQueueNew (1, sizeof(uint8_t), &H7_attributes);

  /* creation of H8 */
  H8Handle = osMessageQueueNew (1, sizeof(uint8_t), &H8_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Chaoshenbo */
  ChaoshenboHandle = osThreadNew(StartChaoshenbo, NULL, &Chaoshenbo_attributes);

  /* creation of Huidu */
  HuiduHandle = osThreadNew(StartHuidu, NULL, &Huidu_attributes);

  /* creation of Tuoluoyi */
  TuoluoyiHandle = osThreadNew(StartTuoluoyi, NULL, &Tuoluoyi_attributes);

  /* creation of Yundong */
  YundongHandle = osThreadNew(StartYundong, NULL, &Yundong_attributes);

  /* creation of Lanyatouchuan */
  LanyatouchuanHandle = osThreadNew(StartLanyatouchuan, NULL, &Lanyatouchuan_attributes);

  /* creation of Yaokong */
  YaokongHandle = osThreadNew(StartYaokong, NULL, &Yaokong_attributes);

  /* creation of Qiehuan */
  QiehuanHandle = osThreadNew(StartQiehuan, NULL, &Qiehuan_attributes);

  /* creation of Jieshou */
  JieshouHandle = osThreadNew(StartJieshou, NULL, &Jieshou_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 9999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 99;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 99;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// ===== DMA 非阻塞 printf =====
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1) {
        debug_tx_busy = 0;  // DMA 发送完成
    }
}

void debug_printf(const char *fmt, ...)
{
    if(debug_tx_busy) return;  // 上次 DMA 还没发完，丢掉这一帧
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(debug_tx_buf, sizeof(debug_tx_buf), fmt, args);
    va_end(args);
    
    uint16_t len = strlen(debug_tx_buf);
    if(len > 0) {
        debug_tx_busy = 1;
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)debug_tx_buf, len);
    }
}

// ===== 阻塞 printf（保留，某些库代码可能还会调用） =====
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
  return ch;
}

int fgetc(FILE *f)
{
  uint8_t ch;
  HAL_UART_Receive(&huart1, &ch, 1, 100);
  return ch;
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartChaoshenbo */
/**
  * @brief  Function implementing the Chaoshenbo thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartChaoshenbo */
void StartChaoshenbo(void *argument)
{
  /* USER CODE BEGIN 5 */
  float distance=0;
  /* Infinite loop */
  for(;;)
  {
    if(yaokong_flag==0)
    {
     distance = chaoshenboceju();
    debug_printf("chaoshenbo=%04d", (int)distance);   //在串口监视器上显示距离值，方便调试
    if(distance<15)
    {
      osDelay(10);
      distance = chaoshenboceju();    //再次测距确认
      if(distance<15)
      {
        osSemaphoreRelease(JuliHandle);   //释放距离信号量，通知Yundong线程进行避障
      }
    }
  }
    osDelay(60);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartHuidu */
/**
* @brief Function implementing the Huidu thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHuidu */
void StartHuidu(void *argument)
{
  /* USER CODE BEGIN StartHuidu */
  /* Infinite loop */
  for(;;)
  {
    if(yaokong_flag==0)
    {
      
    uint32_t zhi_1=zonghuiduzhi();
    osMessageQueuePut(ZonghuiduzhiHandle, &zhi_1, 0, 0);    //将总灰度值放入消息队列，供Yundong线程使用
		
    H1=huiduzhi(0);
    osMessageQueuePut(H1Handle, (void*)&H1, 0, 0);
    H2=huiduzhi(1);
    osMessageQueuePut(H2Handle, (void*)&H2, 0, 0);
    H3=huiduzhi(2);
    osMessageQueuePut(H3Handle, (void*)&H3, 0, 0);
    H4=huiduzhi(3);
    osMessageQueuePut(H4Handle, (void*)&H4, 0, 0);
    H5=huiduzhi(4);
    osMessageQueuePut(H5Handle, (void*)&H5, 0, 0);
    H6=huiduzhi(5);
    osMessageQueuePut(H6Handle, (void*)&H6, 0, 0);
    H7=huiduzhi(6);
    osMessageQueuePut(H7Handle, (void*)&H7, 0, 0);
    H8=huiduzhi(7);
    osMessageQueuePut(H8Handle, (void*)&H8, 0, 0);    //将8个灰度传感器通道的读取值放入消息队列

    }
    osDelay(5);
  }
  /* USER CODE END StartHuidu */
}

/* USER CODE BEGIN Header_StartTuoluoyi */
/**
* @brief Function implementing the Tuoluoyi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTuoluoyi */
void StartTuoluoyi(void *argument)
{
  /* USER CODE BEGIN StartTuoluoyi */
  /* Infinite loop */
  for(;;)
  {
    if(yaokong_flag==0)
    {
      
        MPU6050_Data data;
    MPU6050_ReadFiltered(&data);
    AY=data.raw_accel_y;
    osMessageQueuePut(AYHandle, (void*)&AY, 0, 0);
    AX=data.raw_accel_x;
    osMessageQueuePut(AXHandle, (void*)&AX, 0, 0);
    GZ=data.raw_gyro_z;
    osMessageQueuePut(GZHandle, (void*)&GZ, 0, 0);    //将加速度和陀螺仪原始值放入消息队列

    // 使用原始ADC值判断是否倾斜
    int16_t raw_ay = data.raw_accel_y;
    if(raw_ay>4000||raw_ay<-4000)
    {
      osDelay(10);
      MPU6050_ReadFiltered(&data);    //再次读取数据确认倾斜
      raw_ay = data.raw_accel_y;
      if(raw_ay>4000||raw_ay<-4000)
      {
        MPU6050_ReadFiltered(&data);
        raw_ay = data.raw_accel_y;
        if(raw_ay>4000||raw_ay<-4000)
        {
          Dianjisudu(0, 0);
          osDelay(100);
          Zhizou();   //进入直走状态，等待外部干预
        }
      }
    }
  }
    osDelay(10);
  }
  /* USER CODE END StartTuoluoyi */
}

/* USER CODE BEGIN Header_StartYundong */
/**
* @brief Function implementing the Yundong thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartYundong */
void StartYundong(void *argument)
{
  /* USER CODE BEGIN StartYundong */
  
  /* Infinite loop */
  for(;;)
  {
    if(yaokong_flag==0)
    {
      
    if(osSemaphoreAcquire(JuliHandle, 0) == osOK)
    {
      Xuanzhuan(180);   //如果距离小于15cm，先原地转180度
      osDelay(500);
    }
    else {
      if(osMessageQueueGet(ZonghuiduzhiHandle, (void*)&zhi_2, NULL, 0) == osOK)
      {
        /*switch (zhi_2) {
      case h_00001111: case h_00011111: Xuanzhuan(-90); break;
      case h_11110000: case h_11111000: Xuanzhuan(90); break;
      case h_00010000: Dianjisudu(yuansudu, yuansudu+5); break;
      case h_00001000: Dianjisudu(yuansudu+5, yuansudu); break;
      case h_00000100: case h_00001100: Dianjisudu(yuansudu+15, yuansudu); break;
      case h_00110000: case h_00100000: Dianjisudu(yuansudu, yuansudu+15); break;
      case h_01000000: case h_01100000: Dianjisudu(yuansudu-15, yuansudu+20); break;
      case h_00000010: case h_00000110: Dianjisudu(yuansudu+20, yuansudu-15); break;
      case h_00000001: case h_00000011: Dianjisudu(yuansudu+30, yuansudu-25); break;
      case h_11000000: case h_10000000: Dianjisudu(yuansudu-25, yuansudu+30); break;
      default: Dianjisudu(yuansudu, yuansudu); break;
    }*/
    xunji(zhi_2);   //根据总灰度值执行对应的运动命令
      }
  }
}
  
  osDelay(15);
}
  /* USER CODE END StartYundong */
}

/* USER CODE BEGIN Header_StartLanyatouchuan */
/**
* @brief Function implementing the Lanyatouchuan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLanyatouchuan */
void StartLanyatouchuan(void *argument)
{
  /* USER CODE BEGIN StartLanyatouchuan */
  /* Infinite loop */
  for(;;)
  {
    if(yaokong_flag==0)
    {
      
    /*if(osMessageQueueGet(H1Handle, (void*)&h1, NULL, 0) == osOK &&
       osMessageQueueGet(H2Handle, (void*)&h2, NULL, 0) == osOK &&
       osMessageQueueGet(H3Handle, (void*)&h3, NULL, 0) == osOK &&
       
       osMessageQueueGet(H4Handle, (void*)&h4, NULL, 0) == osOK &&
       osMessageQueueGet(H5Handle, (void*)&h5, NULL, 0) == osOK &&
       osMessageQueueGet(H6Handle, (void*)&h6, NULL, 0) == osOK &&
       osMessageQueueGet(H7Handle, (void*)&h7, NULL, 0) == osOK &&
       osMessageQueueGet(H8Handle, (void*)&h8, NULL, 0) == osOK)
    {
      printf("[display,0,54,%d]", h1);
      printf("[display,15,54,%d]", h2);
      printf("[display,30,54,%d]", h3);
      printf("[display,45,54,%d]", h4);
      printf("[display,60,54,%d]", h5);
      printf("[display,75,54,%d]", h6);
      printf("[display,90,54,%d]", h7);
      printf("[display,105,54,%d]", h8);    //在串口监视器上显示8个灰度传感器的数值，方便调试
    }
    if(osMessageQueueGet(AXHandle, (void*)&ax, NULL, 0) == osOK &&
       osMessageQueueGet(AYHandle, (void*)&ay, NULL, 0) == osOK &&
       osMessageQueueGet(GZHandle, (void*)&gz, NULL, 0) == osOK)
    {
      printf("[plot,%d,%d,%d]",ax,ay,gz);    //发送加速度和陀螺仪原始值到串口监视器的Plot功能，可以实时观察数据变化趋势
      printf("[display,0,0,AX:%06d]",ax);
	    printf("[display,0,18,AY:%06d]",ay);
	    printf("[display,0,36,GZ:%06d]",gz);   //在串口监视器上显示加速度和陀螺仪原始值，方便调试
    }*/

      debug_printf("[display,0,54,%d],[display,15,54,%d],[display,30,54,%d],[display,45,54,%d],[display,60,54,%d],[display,75,54,%d],[display,90,54,%d],[display,105,54,%d]",h1,h2,h3,h4,h5,h6,h7,h8);
      debug_printf("[plot,%d,%d,%d]",ax,ay,gz);
      debug_printf("[display,0,0,AX:%06d],[display,0,18,AY:%06d],[display,0,36,GZ:%06d]",ax,ay,gz);    //合并显示命令，减少串口输出次数，提高效率

  }
    osDelay(1);
  }
  /* USER CODE END StartLanyatouchuan */
}

/* USER CODE BEGIN Header_StartYaokong */
/**
* @brief Function implementing the Yaokong thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartYaokong */
void StartYaokong(void *argument)
{
  /* USER CODE BEGIN StartYaokong */
  /* Infinite loop */
  for(;;)
  {
    if(yaokong_flag==1)
    {
      
      if (osMutexAcquire(ShoujifasongHandle, osWaitForever) == osOK)    //获取互斥锁，确保安全访问Serial_RxPacket
      {
    if(strcmp((const char*)Serial_RxPacket, "qian") == 0 )		//前进
    {
      Dianjisudu(100, 100);
    }
    else if(strcmp((const char*)Serial_RxPacket, "hou") == 0 )	//后退
    {
      Dianjisudu(-100, -100);
    }
    else if(strcmp((const char*)Serial_RxPacket, "zuozhuan") == 0 )	//左转
    {
      Dianjisudu(-100, 100);
    }
    else if(strcmp((const char*)Serial_RxPacket, "youzhuan") == 0 )	//右转
    {
      Dianjisudu(100, -100);
    }
    else if(strcmp((const char*)Serial_RxPacket, "zuoyi") == 0 )	//停止
    {
      Dianjisudu(50, 100);
    }
    else if(strcmp((const char*)Serial_RxPacket, "youyi") == 0 )	//停止
    {
      Dianjisudu(100, 50);
    }
    else {
      Dianjisudu(0, 0);
    }
    osMutexRelease(ShoujifasongHandle);   //释放互斥锁
  }
}
    osDelay(50);
  }
  /* USER CODE END StartYaokong */
}

/* USER CODE BEGIN Header_StartQiehuan */
/**
* @brief Function implementing the Qiehuan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartQiehuan */
void StartQiehuan(void *argument)
{
  /* USER CODE BEGIN StartQiehuan */
  /* Infinite loop */
  for(;;)
  {
    if (osMutexAcquire(ShoujifasongHandle, 100) == osOK)
    {
    if(strcmp((const char*)Serial_RxPacket, "kai")==0)
    {
      yaokong_flag=1;
    }
    else if(strcmp((const char*)Serial_RxPacket, "guan")==0)
    {
      yaokong_flag=0;
    }
    osMutexRelease(ShoujifasongHandle);
  }
    osDelay(50);
  }
  /* USER CODE END StartQiehuan */
}

/* USER CODE BEGIN Header_StartJieshou */
/**
* @brief Function implementing the Jieshou thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartJieshou */
void StartJieshou(void *argument)
{
  /* USER CODE BEGIN StartJieshou */
  /* Infinite loop */
  for(;;)
  {
        if(osMutexAcquire( ShoujifasongHandle, 100) == osOK)
    {
      if(RxFlag==1)
      {
        RxFlag=0;
        memset((void*)Serial_RxPacket, 0, sizeof(Serial_RxPacket));  // 清空缓冲区，防止新旧数据混合
        HAL_UART_Receive_DMA(&huart1, (uint8_t*)Serial_RxPacket, sizeof(Serial_RxPacket));  // 重新启动DMA接收，准备接收下一条命令
      }
    osMutexRelease(ShoujifasongHandle);
    }
    osDelay(50);
  }
  /* USER CODE END StartJieshou */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM10 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM10)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
