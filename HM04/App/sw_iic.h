#ifndef _SW_IIC_H
#define _SW_IIC_H


#include "stm32f1xx_hal.h"
#include "main.h"


#define SW_I2C_WR	0		/* д����bit */
#define SW_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define SW_GPIO_PORT_I2C	HT_SDA_2_GPIO_Port			/* GPIO�˿� */
//#define SW_RCC_I2C_PORT 	RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define SW_I2C_SCL_PIN		HT_SCL_2_Pin			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define SW_I2C_SDA_PIN		HT_SDA_2_Pin			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define SW_I2C_SCL_1()  GPIO_SetBits(SW_GPIO_PORT_I2C, SW_I2C_SCL_PIN)		/* SCL = 1 */
	#define SW_I2C_SCL_0()  GPIO_ResetBits(SW_GPIO_PORT_I2C, SW_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define SW_I2C_SDA_1()  GPIO_SetBits(SW_GPIO_PORT_I2C, SW_I2C_SDA_PIN)		/* SDA = 1 */
	#define SW_I2C_SDA_0()  GPIO_ResetBits(SW_GPIO_PORT_I2C, SW_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define SW_I2C_SDA_READ()  GPIO_ReadInputDataBit(SW_GPIO_PORT_I2C, SW_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define SW_I2C_SCL_1()  SW_GPIO_PORT_I2C->BSRR = SW_I2C_SCL_PIN				/* SCL = 1 */
	#define SW_I2C_SCL_0()  SW_GPIO_PORT_I2C->BRR = SW_I2C_SCL_PIN				/* SCL = 0 */
	
	#define SW_I2C_SDA_1()  SW_GPIO_PORT_I2C->BSRR = SW_I2C_SDA_PIN				/* SDA = 1 */
	#define SW_I2C_SDA_0()  SW_GPIO_PORT_I2C->BRR = SW_I2C_SDA_PIN				/* SDA = 0 */
	
	#define SW_I2C_SDA_READ()  ((SW_GPIO_PORT_I2C->IDR & SW_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

void SW_I2C_Config(void);

void SW_I2C_Start(void);
void SW_I2C_Stop(void);
void SW_I2C_SendByte(uint8_t _ucByte);
uint8_t SW_I2C_ReadByte(void);
uint8_t SW_I2C_WaitAck(void);
void SW_I2C_Ack(void);
void SW_I2C_NAck(void);
uint8_t SW_I2C_CheckDevice(uint8_t _Address);




//#define EEPROM_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
//#define EEPROM_PAGE_SIZE		  8			  /* 24xx02��ҳ���С */
//#define EEPROM_SIZE				  256			  /* 24xx02������ */

//uint8_t EE_CheckOk(void);
//uint8_t EE_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
//uint8_t EE_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
//void EE_Erase(void);
//uint8_t EE_Test(void);

#define ADDRESS_HDC1080 0X80
#define ADDRESS_TEMPERATURE 0X00
#define ADDRESS_HUMIDITY 0X01
#define ADDRESS_CONFIGURATION 0X02



uint8_t InitHDC1080_sw_iic(void);
uint8_t ReadHDC1080_sw_iic(uint16_t *temperature, uint16_t *humidity);
//float CalculateHumidity(uint16_t humidity_from_hdc1080);
//float CalculateTemperature(uint16_t temperature_from_hdc1080);


#endif

