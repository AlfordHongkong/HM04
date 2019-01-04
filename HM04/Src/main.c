
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
  * Copyright (c) 2019 STMicroelectronics International N.V. 
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
#include "api_sensor.h"
#include "sw_iic.h"
#include "gizwits.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
#include "ir_decode.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
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
osThreadId ScenarioTaskHandle;
osMessageQId eventsQueueHandle;
osTimerId PairingHmiTimerHandle;
osTimerId LampDynamicTimerHandle;
osTimerId MistTimerHandle;
osTimerId MistIntermittentTimerHandle;
osTimerId TurnOffMistingDelayTimerHandle;
osTimerId PairingTimerHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
IR_Frame_TypeDef IF_FRAME;

const uint8_t firmware_version[] = "2.2.2";
const uint8_t firmware_version_1 = 2;
const uint8_t firmware_version_2 = 2;
const uint8_t firmware_version_3 = 2;
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
void StartDefaultTask(void const * argument);
void StartLightsTask(void const * argument);
void StartCliTask(void const * argument);
void StartFsmTask(void const * argument);
void StartKeysTask(void const * argument);
void StartGizwitsTask(void const * argument);
void StartSensorsTask(void const * argument);
void StartScenarioTask(void const * argument);
void PairingHmiCallback(void const * argument);
void LampDynamicCallback(void const * argument);
void MistTimerCallback(void const * argument);
void IntermittentCallback(void const * argument);
void TurnOffMistingCallback(void const * argument);
void PairingTimerCallback(void const * argument);                                    
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
  /* USER CODE BEGIN 2 */
  vRegisterCLICommands();
  //InitLampPWM();
  //StartSystemMessage();
  // TestLampPwm();
  
  InitHM04();
  InitGizwits();
  InitHDC1080_sw_iic();
  IR_Init();
  
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
  // HAL_TIM_Base_Start(&htim4);
  HAL_TIM_Base_Start_IT(&htim4);
  // gizwitsSetMode(WIFI_SOFTAP_MODE);  

    // __HAL_RCC_DBGMCU_CLK_ENABLE();
    
  //  HAL_DBGMCU_EnableDBGStandbyMode();
  //  HAL_DBGMCU_EnableDBGStopMode();
  //  __HAL_DBGMCU_FREEZE_TIM4();

  //  DBGMCU->APB1FZ |= 1;
  SystemStartSignal();
  printf("system started.\n");
  printf("Firmware version: %d %d %d\n", firmware_version_1, firmware_version_2, firmware_version_3);
  
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

  /* definition and creation of PairingTimer */
  osTimerDef(PairingTimer, PairingTimerCallback);
  PairingTimerHandle = osTimerCreate(osTimer(PairingTimer), osTimerOnce, NULL);

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

  /* definition and creation of ScenarioTask */
  osThreadDef(ScenarioTask, StartScenarioTask, osPriorityIdle, 0, 128);
  ScenarioTaskHandle = osThreadCreate(osThread(ScenarioTask), NULL);

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

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 71;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 15000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchronization(&htim4, &sSlaveConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

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
  huart1.Init.BaudRate = 9600;
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
  HAL_GPIO_WritePin(GPIOA, led_wifi_Pin|led_bt_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, led_on_Pin|led_int_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(fan_GPIO_Port, fan_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, mist_Pin|HT_SDA_Pin|HT_SCL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, HT_SCL_2_Pin|HT_SDA_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : key_pair_Pin */
  GPIO_InitStruct.Pin = key_pair_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(key_pair_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : led_wifi_Pin led_bt_Pin mist_Pin HT_SDA_Pin 
                           HT_SCL_Pin */
  GPIO_InitStruct.Pin = led_wifi_Pin|led_bt_Pin|mist_Pin|HT_SDA_Pin 
                          |HT_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : led_on_Pin led_int_Pin fan_Pin */
  GPIO_InitStruct.Pin = led_on_Pin|led_int_Pin|fan_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : key_mist_Pin key_lamp_Pin */
  GPIO_InitStruct.Pin = key_mist_Pin|key_lamp_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : HT_SCL_2_Pin HT_SDA_2_Pin */
  GPIO_InitStruct.Pin = HT_SCL_2_Pin|HT_SDA_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  event_t new;
  printf("Rtos started.\n");
  /* Infinite loop */
  for(;;)
  {
    if (IR_Decode(&IF_FRAME) == 0){
      switch(IF_FRAME.Command){
        case COMMAND_LAMP_BUTTON:
        new = EVENT_IR_LAMP;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_MIST_TIMER:
        new = EVENT_IR_MIST;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_WHITE:
        new = EVENT_IR_WHITE;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_YELLOW:
        new = EVENT_IR_YELLOW;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_DYNAMIC:
        new = EVENT_IR_DYNAMIC;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_SCENARIO:
        new = EVENT_IR_SCENARIO;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_VOLUME_PLUS:
        new = EVENT_IR_VOLUME_PLUS;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_VOLUME_MINUS:
        new = EVENT_IR_VOLUME_MINUS;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        case COMMAND_PLAY_PAUSE:
        new = EVENT_WIFI_SPEAKER_PLAY_PAUSE;
        xQueueSend(eventsQueueHandle, &new, 10);
        break;
        default:
        break;

      }
    }
    
    osDelay(100);
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
    osDelay(100);
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
    UpdateTempHumi(&temperature, &humidity);
    osDelay(500);
  }
  /* USER CODE END StartSensorsTask */
}

/* StartScenarioTask function */
void StartScenarioTask(void const * argument)
{
  /* USER CODE BEGIN StartScenarioTask */
  extern Scenario_t Scenario;

  /* Infinite loop */
  for(;;)
  {
    if (scenario_mode == GetLampMode()){
      if (Scenario()){

      }
    }
    osThreadSuspend(ScenarioTaskHandle);
  }
  /* USER CODE END StartScenarioTask */
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

/* PairingTimerCallback function */
void PairingTimerCallback(void const * argument)
{
  /* USER CODE BEGIN PairingTimerCallback */
  StopPairing_hmi();
  /* USER CODE END PairingTimerCallback */
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
// ////////////////
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  else if(htim->Instance == TIM_GIZWITS)
	{
			// keyHandle((keysTypedef_t *)&keys);
			gizTimerMs();
	}
  else if (htim->Instance == TIM4) {
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
