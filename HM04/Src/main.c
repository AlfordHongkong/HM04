
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "bsp_gpio.h"
#include "bsp_lamp.h"
#include "app.h"
#include "fsm.h"

#include "api_hm04.h"
#include "api_lamp.h"
#include "api_mist.h"
#include "sw_iic.h"
#include "gizwits.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
#include "ir_decode.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osThreadId LightsTaskHandle;
osThreadId CliTaskHandle;
osThreadId FsmTaskHandle;
osThreadId KeysTaskHandle;
osThreadId GizwitsTaskHandle;
osThreadId SensorsTaskHandle;
osMessageQId eventsQueueHandle;
osTimerId PairingHmiTimerHandle;
osTimerId LampDynamicTimerHandle;
osTimerId MistTimerHandle;
osTimerId MistIntermittentTimerHandle;
osTimerId TurnOffMistingDelayTimerHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void const * argument);
void StartLightsTask(void const * argument);
void StartCliTask(void const * argument);
void StartFsmTask(void const * argument);
void StartKeysTask(void const * argument);
void StartGizwitsTask(void const * argument);
void StartSensorsTask(void const * argument);
void PairingHmiCallback(void const * argument);
void LampDynamicCallback(void const * argument);
void MistTimerCallback(void const * argument);
void IntermittentCallback(void const * argument);
void TurnOffMistingCallback(void const * argument);
                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
extern void prvUARTCommandConsoleTask(void const * pvParameters );
extern void vRegisterCLICommands( void );
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  vRegisterCLICommands();
  //InitLampPWM();

  // TurnOnLed(led_wifi);
  // TurnOnLed(led_on);
  // TurnOnLed(led_1h);
  // TurnOnLed(led_2h);
  // TurnOffLed(led_wifi);
  // TurnOffLed(led_on);
  // TurnOffLed(led_1h);
  // TurnOffLed(led_2h);
  
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of PairingHmiTimer */
  osTimerDef(PairingHmiTimer, PairingHmiCallback);
  PairingHmiTimerHandle = osTimerCreate(osTimer(PairingHmiTimer), osTimerPeriodic, NULL);

  /* definition and creation of LampDynamicTimer */
  osTimerDef(LampDynamicTimer, LampDynamicCallback);
  LampDynamicTimerHandle = osTimerCreate(osTimer(LampDynamicTimer), osTimerPeriodic, NULL);

  /* definition and creation of MistTimer */
  osTimerDef(MistTimer, MistTimerCallback);
  MistTimerHandle = osTimerCreate(osTimer(MistTimer), osTimerOnce, NULL);

  /* definition and creation of MistIntermittentTimer */
  osTimerDef(MistIntermittentTimer, IntermittentCallback);
  MistIntermittentTimerHandle = osTimerCreate(osTimer(MistIntermittentTimer), osTimerPeriodic, NULL);

  /* definition and creation of TurnOffMistingDelayTimer */
  osTimerDef(TurnOffMistingDelayTimer, TurnOffMistingCallback);
  TurnOffMistingDelayTimerHandle = osTimerCreate(osTimer(TurnOffMistingDelayTimer), osTimerOnce, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LightsTask */
  osThreadDef(LightsTask, StartLightsTask, osPriorityIdle, 0, 128);
  LightsTaskHandle = osThreadCreate(osThread(LightsTask), NULL);

  /* definition and creation of CliTask */
  osThreadDef(CliTask, StartCliTask, osPriorityIdle, 0, 128);
  CliTaskHandle = osThreadCreate(osThread(CliTask), NULL);

  /* definition and creation of FsmTask */
  osThreadDef(FsmTask, StartFsmTask, osPriorityIdle, 0, 128);
  FsmTaskHandle = osThreadCreate(osThread(FsmTask), NULL);

  /* definition and creation of KeysTask */
  osThreadDef(KeysTask, StartKeysTask, osPriorityNormal, 0, 128);
  KeysTaskHandle = osThreadCreate(osThread(KeysTask), NULL);

  /* definition and creation of GizwitsTask */
  osThreadDef(GizwitsTask, StartGizwitsTask, osPriorityNormal, 0, 128);
  GizwitsTaskHandle = osThreadCreate(osThread(GizwitsTask), NULL);

  /* definition and creation of SensorsTask */
  osThreadDef(SensorsTask, StartSensorsTask, osPriorityIdle, 0, 128);
  SensorsTaskHandle = osThreadCreate(osThread(SensorsTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of eventsQueue */
/* what about the sizeof here??? cd native code */
  osMessageQDef(eventsQueue, 16, uint16_t);
  eventsQueueHandle = osMessageCreate(osMessageQ(eventsQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

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
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 29;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 255;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 29;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 255;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct;

  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
  
  /**TIM4 GPIO Configuration  
  PB6   ------> TIM4_CH1 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* TIM4 interrupt Init */
  NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(TIM4_IRQn);

  TIM_InitStruct.Prescaler = 23;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM4, &TIM_InitStruct);

  LL_TIM_DisableARRPreload(TIM4);

  LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);

  LL_TIM_SetTriggerInput(TIM4, LL_TIM_TS_TI1FP1);

  LL_TIM_SetSlaveMode(TIM4, LL_TIM_SLAVEMODE_RESET);

  LL_TIM_IC_SetFilter(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);

  LL_TIM_IC_SetPolarity(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);

  LL_TIM_DisableIT_TRIG(TIM4);

  LL_TIM_DisableDMAReq_TRIG(TIM4);

  LL_TIM_SetTriggerOutput(TIM4, LL_TIM_TRGO_RESET);

  LL_TIM_DisableMasterSlaveMode(TIM4);

  LL_TIM_IC_SetActiveInput(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);

  LL_TIM_IC_SetPrescaler(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);

  LL_TIM_IC_SetActiveInput(TIM4, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_INDIRECTTI);

  LL_TIM_IC_SetPrescaler(TIM4, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);

  LL_TIM_IC_SetFilter(TIM4, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);

  LL_TIM_IC_SetPolarity(TIM4, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);

}

/* TIM7 init function */
static void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 71;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* UART4 init function */
static void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, led_wifi_Pin|led_on_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, led_1h_Pin|led_2h_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(fan_GPIO_Port, fan_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, mist_Pin|HT_SDA_Pin|HT_SCL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : key_pair_Pin */
  GPIO_InitStruct.Pin = key_pair_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(key_pair_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : led_wifi_Pin led_on_Pin mist_Pin HT_SDA_Pin 
                           HT_SCL_Pin */
  GPIO_InitStruct.Pin = led_wifi_Pin|led_on_Pin|mist_Pin|HT_SDA_Pin 
                          |HT_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : led_1h_Pin led_2h_Pin fan_Pin */
  GPIO_InitStruct.Pin = led_1h_Pin|led_2h_Pin|fan_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : key_mist_Pin key_lamp_Pin */
  GPIO_InitStruct.Pin = key_mist_Pin|key_lamp_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  //StartSystemMessage();
  // TestLampPwm();

  InitHM04();
  InitGizwits();
  InitHDC1080_sw_iic();
  IR_Init();
  // gizwitsSetMode(WIFI_SOFTAP_MODE);
  /* Infinite loop */
  for(;;)
  {
    // TurnOffAllLamps();
    // osDelay(500);
    // TurnOnAllLamps();
    // ToggleLed(led_1h);
    // ToggleLed(led_2h);
    // ToggleLed(led_on);
    // ToggleLed(led_wifi);
    osDelay(500);
  }
  /* USER CODE END 5 */ 
}

/* StartLightsTask function */
void StartLightsTask(void const * argument)
{
  /* USER CODE BEGIN StartLightsTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END StartLightsTask */
}

/* StartCliTask function */
void StartCliTask(void const * argument)
{
  /* USER CODE BEGIN StartCliTask */
  /* Infinite loop */
  //prvUARTCommandConsoleTask(NULL);
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END StartCliTask */
}

/* StartFsmTask function */
void StartFsmTask(void const * argument)
{
  /* USER CODE BEGIN StartFsmTask */
  /* Infinite loop */
  for(;;)
  {
    StartFSM();
    osDelay(10);
  }
  /* USER CODE END StartFsmTask */
}

/* StartKeysTask function */
void StartKeysTask(void const * argument)
{
  /* USER CODE BEGIN StartKeysTask */
  //key_state_t * key_state;
  //uint8_t * keys_acc;
  /* Infinite loop */
  for(;;)
  {
    ReadKeys(GetReadKeysFuncs(), KeysCallback);
    osDelay(50);
  }
  /* USER CODE END StartKeysTask */
}

/* StartGizwitsTask function */
void StartGizwitsTask(void const * argument)
{
  /* USER CODE BEGIN StartGizwitsTask */
  extern dataPoint_t currentDataPoint;
  /* Infinite loop */
  for(;;)
  {
    userHandle();
    gizwitsHandle((dataPoint_t *)&currentDataPoint);
    osDelay(50);
  }
  /* USER CODE END StartGizwitsTask */
}

/* StartSensorsTask function */
void StartSensorsTask(void const * argument)
{
  /* USER CODE BEGIN StartSensorsTask */
  float temperature, humidity;
  /* Infinite loop */
  for(;;)
  {
    GetTemperatureHumidity(&temperature, &humidity);
    SetHDC1080(&temperature, &humidity);
    osDelay(100);
  }
  /* USER CODE END StartSensorsTask */
}

/* PairingHmiCallback function */
void PairingHmiCallback(void const * argument)
{
  /* USER CODE BEGIN PairingHmiCallback */
  ToggleLed(led_wifi);
  /* USER CODE END PairingHmiCallback */
}

/* LampDynamicCallback function */
void LampDynamicCallback(void const * argument)
{
  /* USER CODE BEGIN LampDynamicCallback */
  LampDynamicCallbackFromApiLamp();
  /* USER CODE END LampDynamicCallback */
}

/* MistTimerCallback function */
void MistTimerCallback(void const * argument)
{
  /* USER CODE BEGIN MistTimerCallback */
  MistingTimerCallback_api_mist();
  /* USER CODE END MistTimerCallback */
}

/* IntermittentCallback function */
void IntermittentCallback(void const * argument)
{
  /* USER CODE BEGIN IntermittentCallback */
  MistingIntermittentCallback_api_mist();
  /* USER CODE END IntermittentCallback */
}

/* TurnOffMistingCallback function */
void TurnOffMistingCallback(void const * argument)
{
  /* USER CODE BEGIN TurnOffMistingCallback */
  StoppingMistingDelayCallback_api_mist();
  /* USER CODE END TurnOffMistingCallback */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  else if(htim->Instance == TIM_GIZWITS) ///< for gizwits timer
  {
    gizTimerMs();
  }
  else if(htim->Instance == TIM4){  ///< timer4 is for pwm input mode
    IR_ResetPacket();
  }

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
