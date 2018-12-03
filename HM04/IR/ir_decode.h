/**
  ******************************************************************************
  * @file    InfraRed/IR_Decoding_PWMI/inc/ir_decode.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    25-January-2012
  * @brief   This file contains all the functions prototypes for the IR Decode 
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IR_DECODE_H
#define __IR_DECODE_H

#ifdef __cplusplus
 extern "C" {
#endif

#if !defined (IR_RC5_PROTOCOL) \
&& !defined (IR_SIRC_PROTOCOL) \
&& !defined (IR_NEC_PROTOCOL) \
&& !defined (IR_ALFORD_PROTOCOL)
//#define IR_RC5_PROTOCOL
//#define IR_SIRC_PROTOCOL
#define IR_NEC_PROTOCOL
// #define IR_ALFORD_PROTOCOL
#endif 
   
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
//#include "stm32100e_eval_lcd.h"
#include <stdio.h>

#if defined IR_RC5_PROTOCOL
  #include "rc5_decode.h"
#elif defined  IR_SIRC_PROTOCOL
  #include "sirc_decode.h"
#elif defined IR_NEC_PROTOCOL
#include "nec_decode.h"
#elif defined IR_ALFORD_PROTOCOL
#include "alford_decode.h"
#endif

#if defined IR_ALFORD_PROTOCOL
#define COMMAND_11 0X04
#define COMMAND_12 0X05
#define COMMAND_13 0X09
#define COMMAND_14 0X08
#define COMMAND_21 0X07
#define COMMAND_31 0X06
#define COMMAND_41 0X02
#define COMMAND_42 0X03
#define COMMAND_43 0X01
#elif defined IR_NEC_PROTOCOL
#define COMMAND_11 0X80
#define COMMAND_12 0XC0
#define COMMAND_13 0X20
#define COMMAND_14 0X40
#define COMMAND_21 0XA0
#define COMMAND_31 0X60
#define COMMAND_41 0XE0
#define COMMAND_42 0X10
#define COMMAND_43 0XFF
#endif


#define COMMAND_LAMP_BUTTON COMMAND_11
#define COMMAND_MIST_TIMER  COMMAND_14
#define COMMAND_WHITE       COMMAND_12
#define COMMAND_YELLOW      COMMAND_13
#define COMMAND_DYNAMIC     COMMAND_21
#define COMMAND_SCENARIO    COMMAND_31
#define COMMAND_VOLUME_PLUS      COMMAND_41
#define COMMAND_VOLUME_MINUS     COMMAND_43
#define COMMAND_PLAY_PAUSE  COMMAND_42

/** @addtogroup STM32F10x_Infrared_Decoders
  * @{
  */

/** @addtogroup IR_Decoding_PWMI
  * @{
  */

/** @addtogroup IR_DECODE
  * @{
  */

/** @defgroup IR_DECODE_Exported_Types
  * @{
  */

/** @defgroup IR_DECODE_Exported_Constants
  * @{
  */
   
//#define IR_TIM                 TIM4                     /*!< Timer used for IR decoding */
//#define TIM_PRESCALER          23                       /* !< TIM prescaler */
//#define IR_TIM_CLK             RCC_APB1Periph_TIM4      /*!< Clock of the used timer */
//#define IR_TIM_IRQn            TIM4_IRQn             /*!< IR TIM IRQ */
//#define IR_TIM_Channel         TIM_Channel_1            /*!< IR TIM Channel */
//
//#define IR_GPIO_PORT           GPIOB                    /*!< Port which IR output is connected */
//#define IR_GPIO_PORT_CLK       RCC_APB2Periph_GPIOB     /*!< IR pin GPIO Clock Port */
//#define IR_GPIO_PIN            GPIO_Pin_6               /*!< Pin which IR is connected */
/**
  * @}
  */

/** @defgroup IR_DECODE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup IR_DECODE_Exported_Functions
  * @{
  */
void IR_DeInit(void);
void IR_Init(void);
uint8_t IR_Decode(IR_Frame_TypeDef *ir_frame);
void IR_ResetPacket(void);

#ifdef __cplusplus
}
#endif

#endif /* __IR_DECODE_H */
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
