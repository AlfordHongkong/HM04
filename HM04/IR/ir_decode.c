/**
  ******************************************************************************
  * @file    InfraRed/IR_Decoding_PWMI/src/ir_decode.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    25-January-2012
  * @brief   This file provides all the InfraRed Decode firmware functions.
  *    
  * 1. How to use this driver
  * -------------------------         
  *      - TIM3 Capture Compare and Update interrupts are used to decode the IR
  *        frame, if a frame is received correctly a global variable IRFrameReceived 
  *        will be set to inform the application.
  *          
  *      - Then the application should call the function IR_Decode() to retrieve 
  *        the received IR frame. 
  *        
  *      - You can easily tailor this driver to any other InfraRed protocol, 
  *        by just adapting the defines from sirc_decode.h to the InfraRed 
  *        protocol specification(Bit Duration, Header Duration, Marge Tolerance, 
  *        Number of bits...) and the command and device tables from ir_commands.c.         
  *                   
  * 2. Important to know
  * --------------------  
  *      - TIM3_IRQHandler ISRs are coded within this driver.
  *        If you are using one or both Interrupts in your application you have 
  *        to proceed carefully:   
  *           - either add your application code in these ISRs
  *           - or copy the contents of these ISRs in you application code
  *                              
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * FOR MORE INFORMATION PLEASE READ CAREFULLY THE LICENSE AGREEMENT FILE
  * LOCATED IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#include "ir_decode.h"
// #include "stm32100e_eval_lcd.h"
#include "ir_commands.c"

extern TIM_HandleTypeDef htim4;

/** @addtogroup STM32F10x_Infrared_Decoders
  * @{
  */

/** @addtogroup IR_Decoding_PWMI
  * @{
  */

/** @addtogroup IR_DECODE
  * @brief IR_DECODE driver modules
  * @{
  */

/** @defgroup IR_DECODE_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup IR_DECODE_Private_Defines
  * @{
  */
/**
  * @}  
  */


/** @defgroup IR_DECODE_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup IR_DECODE_Private_Variables
  * @{
  */
__IO StatusYesOrNo IRFrameReceived = NO;   /*!< IR frame status */
  

uint32_t sample_buff[100];

/* IR bit definitions*/
uint16_t IROnTimeMin = 0; 
uint16_t IROnTimeMax = 0; 
uint16_t IRValueStep = 0;
uint16_t IRValueMargin = 0;
uint16_t IRValue00 = 0;

/* Header time definitions*/
uint16_t IRHeaderLowMin = 0;
uint16_t IRHeaderLowMax = 0;
uint16_t IRHeaderWholeMin = 0;
uint16_t IRHeaderWholeMax = 0;
tIR_packet IRTmpPacket; /*!< IR packet*/

uint16_t IRTimeOut = 0;
__IO uint32_t TIMCLKValueKHz = 0;/*!< Timer clock */

/**
  * @}
  */

/** @defgroup IR_DECODE_Private_FunctionPrototypes
  * @{
  */
 
static void IR_DataSampling(uint32_t lowPulseLength, uint32_t wholePulseLength);
static uint8_t IR_DecodeHeader(uint32_t lowPulseLength, uint32_t wholePulseLength);
static uint8_t IR_DecodeBit(uint32_t lowPulseLength, uint32_t wholePulseLength);
static uint32_t TIM_GetCounterCLKValue(void);
 
/**
  * @}
  */

/** @defgroup IR_DECODE_Private_Functions
  * @{
  */

/**
  * @brief  De-initializes the peripherals (RCC,GPIO, TIM)       
  * @param  None
  * @retval None
  */
void IR_DeInit(void)
{ 
  // TIM_DeInit(IR_TIM);
  // GPIO_DeInit(IR_GPIO_PORT);
}

/**
  * @brief  Initialize the IR bit time range.
  * @param  None
  * @retval None
  */
void IR_Init(void)
{ 
  /// set URS bit in TIMx_CR1 to 1: only counter overflow/underflow generates an update
  __HAL_TIM_URS_ENABLE(&htim4);
  /*========  set gpio and nvic  ==========================================*/

  // GPIO_InitTypeDef GPIO_InitStructure;
  // NVIC_InitTypeDef NVIC_InitStructure;
  // TIM_ICInitTypeDef TIM_ICInitStructure;
  
  // /*  Clock Configuration for TIMER */
  // RCC_APB1PeriphClockCmd(IR_TIM_CLK , ENABLE);

  // /* Enable Button GPIO clock */
  // RCC_APB2PeriphClockCmd(IR_GPIO_PORT_CLK | RCC_APB2Periph_AFIO, ENABLE);
  
  /* Pin configuration: input floating */
  // GPIO_InitStructure.Pin = GPIO_PIN_6;
  // GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
  // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  // HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
  // GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
     
  // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the TIM global Interrupt */
  // NVIC_InitStructure.NVIC_IRQChannel = IR_TIM_IRQn ;
  // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  // NVIC_Init(&NVIC_InitStructure);
  // HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
  // HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /*====================================================================*/

  

 /*========== set frequency and UP bit  =================================*/

  /* TIMER frequency input */
  // TIM_PrescalerConfig(IR_TIM, TIM_PRESCALER, TIM_PSCReloadMode_Immediate);

  // htim4.Instance = TIM4;

  // __HAL_TIM_SET_PRESCALER(&htim4, 23);
  // htim4.Instance->EGR |= 0x0001;
  /*====================================================================*/
 
  /*====================================================================*/
  /* Origin: TIM configuration; Update: TIM channel configuration */
  // TIM_ICInitStructure.TIM_Channel = TIM_CHANNEL_1;
  // TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPOLARITY_FALLING; 
  // TIM_ICInitStructure.TIM_ICSelection = TIM_ICSELECTION_DIRECTTI;
  // TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  // TIM_ICInitStructure.TIM_ICFilter = 0x0;
  // TIM_PWMIConfig(&htim4, &TIM_ICInitStructure); 
  // TIM_IC_InitTypeDef TIM_ICInitStructure;
  // TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_FALLING;
  // TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_DIRECTTI;
  // TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
  // TIM_ICInitStructure.ICFilter = 0x0;
  // HAL_TIM_IC_ConfigChannel(&htim4, &TIM_ICInitStructure, TIM_CHANNEL_1);
  // TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
  // TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  // HAL_TIM_IC_ConfigChannel(&htim4, &TIM_ICInitStructure, TIM_CHANNEL_1);
  /*====================================================================*/



  /*====== get timer clock  ===============================================*/
  // /* Timer Clock */
  // TIMCLKValueKHz = TIM_GetCounterCLKValue()/1000; 
  TIMCLKValueKHz = 1000;
  /*====================================================================*/



  /*======================  set slave mode  =================================*/
  // /* Select the TIM3 Input Trigger: TI1FP1 */
  // TIM_SelectInputTrigger(IR_TIM, TIM_TS_TI1FP1);
  

  // /* Select the slave Mode: Reset Mode */
  // TIM_SelectSlaveMode(IR_TIM, TIM_SlaveMode_Reset);

  // /* Enable the Master/Slave Mode */
  // TIM_SelectMasterSlaveMode(IR_TIM, TIM_MasterSlaveMode_Enable);
  // TIM_SlaveConfigTypeDef SlaveConfig;
  // SlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  // SlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  // HAL_TIM_SlaveConfigSynchronization(&htim4, &SlaveConfig);
  // TIM_MasterConfigTypeDef MasterConfig;
  // MasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
  // HAL_TIMEx_MasterConfigSynchronization(&htim4, &MasterConfig);
  /*====================================================================*/



  /*========================= set URS bit  ==============================*/
  // /* Configures the TIM Update Request Interrupt source: counter overflow */
  // TIM_UpdateRequestConfig(IR_TIM,  TIM_UpdateSource_Regular);
  // __HAL_TIM_URS_ENABLE(&htim4);
   /*====================================================================*/



  /*=== set timeout =====================================================*/
  IRTimeOut = TIMCLKValueKHz * IR_TIME_OUT_US/1000;

  /* Set the TIM auto-reload register for each IR protocol */
  // htim4.Instance->ARR = IRTimeOut;
  /*======================================================================*/
  



  /*=== cleanr update flag ============================================*/
  // /* Clear update flag */
  // TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);
  // __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
  /*======================================================================*/




  /*====  enablessss ===================================================*/
  // /* Enable TIM3 Update Event Interrupt Request */
  // TIM_ITConfig(IR_TIM, TIM_IT_Update, ENABLE);

  // /* Enable the CC2/CC1 Interrupt Request */
  // TIM_ITConfig(IR_TIM, TIM_IT_CC2, ENABLE);
  // TIM_ITConfig(IR_TIM, TIM_IT_CC1, ENABLE);

  // /* Enable the timer */
  // TIM_Cmd(IR_TIM, ENABLE);

  // HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  // HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
  // HAL_TIM_Base_Start_IT(&htim4);
  /*========================================================================*/
  /* Header */	
  IRHeaderLowMin = TIMCLKValueKHz * IR_HEADER_LOW_MIN_US/1000;
  IRHeaderLowMax = TIMCLKValueKHz * IR_HEADER_LOW_MAX_US/1000;
  IRHeaderWholeMin = TIMCLKValueKHz * IR_HEADER_WHOLE_MIN_US/1000;
  IRHeaderWholeMax = TIMCLKValueKHz * IR_HEADER_WHOLE_MAX_US/1000;

  /* Bit time range*/
  IROnTimeMax = TIMCLKValueKHz * IR_ONTIME_MAX_US /1000;
  IROnTimeMin = TIMCLKValueKHz * IR_ONTIME_MIN_US/1000; 
  IRValueStep = TIMCLKValueKHz * IR_VALUE_STEP_US/1000;
  IRValueMargin = TIMCLKValueKHz * IR_VALUE_MARGIN_US/1000;
  IRValue00 = TIMCLKValueKHz * IR_VALUE_00_US/1000;

  /* Default state */
  IR_ResetPacket();
}

/**
  * @brief  Decode the IR frame (ADDRESS, COMMAND) when all the frame is 
  *         received, the IRFrameReceived will equal to YES.
  *         The IRFrameReceived is set to YES inside the  IR_DataSampling() 
  *         function when the whole IR frame is received without any error.
  *         If any received bit timing is out of the defined range, the IR packet
  *         is reset and the IRFrameReceived is set to NO.
  *         After the IR received display, the IRFrameReceived is set NO.
  *         User can check the IRFrameReceived variable status to verify if there
  *         is a new IR frame already received.          
  * @param  ir_frame: pointer to IR_Frame_TypeDef structure that contains the 
  *         the IR protocol fields (Address, Command,...).
  * @retval None
  */
void IR_Decode(IR_Frame_TypeDef *ir_frame)
{  
  if(IRFrameReceived != NO) 
  {
    IRTmpPacket.data = __RBIT(IRTmpPacket.data);

#ifdef IR_SIRC_PROTOCOL   
    ir_frame->Command = (IRTmpPacket.data >> 20)& 0x7F;
    ir_frame->Address = (IRTmpPacket.data >> 27) & 0x1F;
#endif

#ifdef IR_NEC_PROTOCOL
    ir_frame->User = (uint8_t)(IRTmpPacket.data >> 24);
    ir_frame->UserInverse = (uint8_t)(IRTmpPacket.data >> 16);
    ir_frame->Command = (uint8_t)(IRTmpPacket.data >> 8);
    ir_frame->CommandInverse = (uint8_t)(IRTmpPacket.data);
#endif
    
    /* Default state */
    IRFrameReceived = NO; 
    IR_ResetPacket();

  }
}

/**
  * @brief  Put the packet to default state, ready to be filled in by incoming 
  *         data.
  * @param  None
  * @retval None
  */
void IR_ResetPacket(void)
{
  IRTmpPacket.count = 0;
  IRTmpPacket.status = INITIAL_STATUS;
  IRTmpPacket.data = 0;

}

/**
  * @brief  This function handles TIM interrupt Handler.
  *         SIRC Infrared decoder Implementation
  *         ===================================  
  *         Capture Compare Interrupt:Timer Falling Edge Event:
  *         ---------------------------------------------------
  *         The Timer interrupt is used to measure the different period between 
  *         two successive falling edges in order to identify the frame bits.
  *
  *         We measure the low pulse duration:
  *           - If the period measured is equal to T = 1200 �s and the low pulse 
  *             duration is equal to T/2 = 600�s => the bit is logic '0'. 
  *           - If the period measured is equal to 3T/2 = 1800�s and the low pulse 
  *             duration is equal to T = 1200�s => the bit is logic '1'.
  *           - If the whole period measured is equal to 3000�s and the low pulse 
  *             duration is equal to 2400�s => the bit is �start bit�.
  *
  *         Update event:InfraRed decoders time out event
  *         ----------------------------------------------
  *         It resets the InfraRed decoders packet.                 
  *         - The Timer Overflow is set to 4 ms. 
  * @param  None
  * @retval None
  */
// void TIM4_IRQHandler (void)
// {
//   static uint32_t ICValue1;
//   static uint32_t ICValue2;
  
//   /* IC1 Interrupt */
//   if((__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_CC1) != RESET))
//   {
//     __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_CC1);
//     /* Get the Input Capture value */
//     // ICValue2 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
//     // IR_DataSampling(ICValue1, ICValue2); 
//   }  /* IC2 Interrupt*/   
//   else  if((__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_CC2) != RESET))
//   {
//     __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_CC2);
//     /* Get the Input Capture value */
//     // ICValue1 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2);
//   } 
//   /* Checks whether the IR_TIM flag is set or not. */
//   else if ((__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) != RESET))
//   { 
//     /* Clears the IR_TIM's pending flags*/
//     __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
    
//     IR_ResetPacket();
//   }
//   else {

//   }
// }

uint32_t ICValue1;
uint32_t ICValue2;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  
  if (htim->Instance == TIM4){
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
      ICValue2 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
      sample_buff[sample_count] = ICValue2;
      IR_DataSampling(ICValue1, ICValue2); 
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
      ICValue1 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2);
      sample_buff[sample_count] = ICValue1;
    }
    else {
      
    }
  }
}

/**
  * @brief  Identify the IR data.
  * @param  lowPulseLength: low pulse duration. 
  * @param  wholePulseLength: whole pulse duration.
  * @retval None
  */
static void IR_DataSampling(uint32_t lowPulseLength, uint32_t wholePulseLength)
{
  uint8_t  tmpBit = 0;
  /* If the pulse timing is correct */
  if ((IRTmpPacket.status == IR_STATUS_RX)) 
  {
    /* Convert raw pulse timing to data value */
    tmpBit = IR_DecodeBit(lowPulseLength, wholePulseLength); 
    if (tmpBit != IR_BIT_ERROR) /* If the pulse timing is correct */
    {
      /* This operation fills in the incoming bit to the correct position in
      32 bit word*/
      IRTmpPacket.data <<= 1;
      IRTmpPacket.data |= tmpBit;
      
      /* Increment the bit count  */
      IRTmpPacket.count++;
    }
    else {
      /// ir bit error
    }

    /* If all bits identified */
    if (IRTmpPacket.count == IR_TOTAL_BITS_COUNT)
    {
      /* Frame received*/
      IRFrameReceived = YES;
    }
    /* Bit 15:the idle time between IR message and the inverted one */
    else if (IRTmpPacket.count == IR_BITS_COUNT)
    {
      IRTmpPacket.status = IR_STATUS_HEADER; 
    }
  }
  else if ((IRTmpPacket.status == IR_STATUS_HEADER))
  {
    /* Convert raw pulse timing to data value */
    tmpBit = IR_DecodeHeader(lowPulseLength, wholePulseLength);
    
    /* If the Header timing is correct */
    if ( tmpBit!= IR_HEADER_ERROR)
    { 
      /* Reception Status for the inverted message */
      IRTmpPacket.status = IR_STATUS_RX; 
    }
    else
    {
      /* Wrong header time */
      IR_ResetPacket();  
    }
  }
}

/**
  * @brief  Check the header pulse if it has correct pulse time.
  * @param  lowPulseLength: low pulse header duration.
  * @param  wholePulseLength: whole pulse header duration.
  * @retval IR_HEADER_OK or IR_HEADER_ERROR
  */
static uint8_t IR_DecodeHeader(uint32_t lowPulseLength, uint32_t wholePulseLength)
{
  /* First check if low pulse time is according to the specs */
  if ((lowPulseLength >= IRHeaderLowMin) && (lowPulseLength <= IRHeaderLowMax)) 
  {
    /* Check if the high pulse is OK */
    if ((wholePulseLength >= IRHeaderWholeMin) && (wholePulseLength <= IRHeaderWholeMax))
    {
      return IR_HEADER_OK; /* Valid Header */
    }
  }	
  return IR_HEADER_ERROR;  /* Wrong Header */
}

/**
  * @brief  Convert raw time to data value.
  * @param  lowPulseLength: low pulse bit duration.
  * @param  wholePulseLength: whole pulse bit duration.
  * @retval BitValue( data0 or data1) or IR_BIT_ERROR
  */
static uint8_t IR_DecodeBit(uint32_t lowPulseLength , uint32_t wholePulseLength)
{
  uint8_t i = 0;
  
  /* First check if low pulse time is according to the specs */
  if ((lowPulseLength >= IROnTimeMin) && (lowPulseLength <= IROnTimeMax))
  {
    for (i = 0; i < 2; i++) /* There are data0 to data1 */
    {
      /* Check if the length is in given range */
      if ((wholePulseLength >= IRValue00 + (IRValueStep * i) - IRValueMargin) 
          && (wholePulseLength <= IRValue00 + (IRValueStep * i) + IRValueMargin)){
          
        return ((i+1) & 0x01); ///< for test
        return i;  /* Return bit value */
      }
        
    }
  }
  return IR_BIT_ERROR; /* No correct pulse length was found */
}

/**
  * @brief  Identify TIM clock
  * @param  None
  * @retval Timer clock
  */
static uint32_t TIM_GetCounterCLKValue(void)
{
  uint32_t apbprescaler = 0, apbfrequency = 0;
  uint32_t timprescaler = 0;
  // __IO RCC_ClocksTypeDef RCC_ClockFreq;
  
  /* This function fills the RCC_ClockFreq structure with the current
  frequencies of different on chip clocks */
  // RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_ClockFreq);
  
  // if((IR_TIM == TIM1) || (IR_TIM == TIM8) || (IR_TIM == TIM15) ||
  //    (IR_TIM == TIM16) || (IR_TIM == TIM17))
  // {    
  //   /* Get the clock prescaler of APB2 */
  //   apbprescaler = ((RCC->CFGR >> 11) & 0x7);
  //   apbfrequency = RCC_ClockFreq.PCLK2_Frequency;
  //   timprescaler = TIM_PRESCALER;
  // }
  // else if((IR_TIM == TIM2) || (IR_TIM == TIM3) || (IR_TIM == TIM4) ||
  //         (IR_TIM == TIM5) || (IR_TIM == TIM6) || (IR_TIM == TIM7) )
  // {
  //   /* Get the clock prescaler of APB1 */
  //   apbprescaler = ((RCC->CFGR >> 8) & 0x7);
  //   apbfrequency = RCC_ClockFreq.PCLK1_Frequency; 
  //   timprescaler = TIM_PRESCALER;
  // }
  
  /* If APBx clock div >= 4 */
  if (apbprescaler >= 4)
  {
    return ((apbfrequency * 2)/(timprescaler + 1));
  }
  else
  {
    return (apbfrequency/(timprescaler+ 1));
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
