#include "sw_iic.h"
//#include "BSP_SysTick.h"
#include "stdio.h"
//#include "BSP_AP360_GPIO.H"





#define ERROR_HDC1080_NO_ACK 1
static void HDC1080_CallBack(uint8_t code);

static void delay_ms(unsigned int ms){
	unsigned i=0;
	while(ms--){
		i=12000;
		while(i--);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void I2C_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SW_I2C_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	SW_I2C_SDA_1();
	SW_I2C_SCL_1();
	I2C_Delay();
	SW_I2C_SDA_0();
	I2C_Delay();
	SW_I2C_SCL_0();
	I2C_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SW_I2C_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	SW_I2C_SDA_0();
	SW_I2C_SCL_1();
	I2C_Delay();
	SW_I2C_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SW_I2C_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			SW_I2C_SDA_1();
		}
		else
		{
			SW_I2C_SDA_0();
		}
		I2C_Delay();
		SW_I2C_SCL_1();
		I2C_Delay();	
		SW_I2C_SCL_0();
		if (i == 7)
		{
			 SW_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		I2C_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t SW_I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		SW_I2C_SCL_1();
		I2C_Delay();
		if (SW_I2C_SDA_READ())
		{
			value++;
		}
		SW_I2C_SCL_0();
		I2C_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t SW_I2C_WaitAck(void)
{
	uint8_t re;

	SW_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	I2C_Delay();
	SW_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	I2C_Delay();
	if (SW_I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	SW_I2C_SCL_0();
	I2C_Delay();
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_Ack(void)
{
	SW_I2C_SDA_0();	/* CPU����SDA = 0 */
	I2C_Delay();
	SW_I2C_SCL_1();	/* CPU����1��ʱ�� */
	I2C_Delay();
	SW_I2C_SCL_0();
	I2C_Delay();
	SW_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SW_I2C_NAck(void)
{
	SW_I2C_SDA_1();	/* CPU����SDA = 1 */
	I2C_Delay();
	SW_I2C_SCL_1();	/* CPU����1��ʱ�� */
	I2C_Delay();
	SW_I2C_SCL_0();
	I2C_Delay();	
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_Config
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SW_I2C_Config(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	RCC_APB2PeriphClockCmd(SW_RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */
//
//	GPIO_InitStructure.GPIO_Pin = SW_I2C_SCL_PIN | SW_I2C_SDA_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
//	GPIO_Init(SW_GPIO_PORT_I2C, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	SW_I2C_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: SW_I2C_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t SW_I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

//	SW_I2C_Config();		/* ����GPIO */

	
	SW_I2C_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	SW_I2C_SendByte(_Address | SW_I2C_WR);
	ucAck = SW_I2C_WaitAck();	/* ����豸��ACKӦ�� */

	SW_I2C_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}


/*
================================================================================================================================================
=======================================EEPROM FUCTIONs==================================================================================================

*****************************************************************************************
*	�� �� ��: EE_CheckOk
*	����˵��: �жϴ���EERPOM�Ƿ�����
*	��    �Σ���
*	�� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
*****************************************************************************************
*/
//uint8_t EE_CheckOk(void)
//{
//	if (SW_I2C_CheckDevice(EEPROM_DEV_ADDR) == 0)
//	{
//		return 1;
//	}
//	else
//	{
//		/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
//		SW_I2C_Stop();		
//		return 0;
//	}
//}

/*
*********************************************************************************************************
*	�� �� ��: EE_ReadBytes
*	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
//uint8_t EE_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
//{
//	uint16_t i;
//	
//	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
//	
//	/* ��1��������I2C���������ź� */
//	SW_I2C_Start();
//	
//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	SW_I2C_SendByte(EEPROM_DEV_ADDR | SW_I2C_WR);	/* �˴���дָ�� */
//	 
//	/* ��3�����ȴ�ACK */
//	if (SW_I2C_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
//	SW_I2C_SendByte((uint8_t)_usAddress);
//	
//	/* ��5�����ȴ�ACK */
//	if (SW_I2C_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	
//	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
//	SW_I2C_Start();
//	
//	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	SW_I2C_SendByte(EEPROM_DEV_ADDR | SW_I2C_RD);	/* �˴��Ƕ�ָ�� */
//	
//	/* ��8��������ACK */
//	if (SW_I2C_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}	
//	
//	/* ��9����ѭ����ȡ���� */
//	for (i = 0; i < _usSize; i++)
//	{
//		_pReadBuf[i] = SW_I2C_ReadByte();	/* ��1���ֽ� */
//		
//		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
//		if (i != _usSize - 1)
//		{
//			I2C_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
//		}
//		else
//		{
//			SW_I2C_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//		}
//	}
//	/* ����I2C����ֹͣ�ź� */
//	SW_I2C_Stop();
//	return 1;	/* ִ�гɹ� */

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	SW_I2C_Stop();
//	return 0;
//}

///*
//*********************************************************************************************************
//*	�� �� ��: EE_WriteBytes
//*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
//*	��    �Σ�_usAddress : ��ʼ��ַ
//*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
//*			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
//*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
//*********************************************************************************************************
//*/
//uint8_t EE_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
//{
//	uint16_t i,m;
//	uint16_t usAddr;
//	
//	/* 
//		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
//		����24xx02��page size = 8
//		�򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ûд1���ֽڣ������͵�ַ
//		Ϊ���������д��Ч��: ����������page wirte������
//	*/

//	usAddr = _usAddress;	
//	for (i = 0; i < _usSize; i++)
//	{
//		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
//		if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
//		{
//			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
//			SW_I2C_Stop();
//			
//			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
//				CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
//			*/
//			for (m = 0; m < 1000; m++)
//			{				
//				/* ��1��������I2C���������ź� */
//				SW_I2C_Start();
//				
//				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//				SW_I2C_SendByte(EEPROM_DEV_ADDR | SW_I2C_WR);	/* �˴���дָ�� */
//				
//				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
//				if (SW_I2C_WaitAck() == 0)
//				{
//					break;
//				}
//			}
//			if (m  == 1000)
//			{
//				goto cmd_fail;	/* EEPROM����д��ʱ */
//			}
//		
//			/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
//			SW_I2C_SendByte((uint8_t)usAddr);
//			
//			/* ��5�����ȴ�ACK */
//			if (SW_I2C_WaitAck() != 0)
//			{
//				goto cmd_fail;	/* EEPROM������Ӧ�� */
//			}
//		}
//	
//		/* ��6������ʼд������ */
//		SW_I2C_SendByte(_pWriteBuf[i]);
//	
//		/* ��7��������ACK */
//		if (SW_I2C_WaitAck() != 0)
//		{
//			goto cmd_fail;	/* EEPROM������Ӧ�� */
//		}

//		usAddr++;	/* ��ַ��1 */		
//	}
//	
//	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
//	SW_I2C_Stop();
//	return 1;

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	SW_I2C_Stop();
//	return 0;
//}


//void EE_Erase(void)
//{
//	uint16_t i;
//	uint8_t buf[EEPROM_SIZE];
//	
//	/* ��仺���� */
//	for (i = 0; i < EEPROM_SIZE; i++)
//	{
//		buf[i] = 0xFF;
//	}
//	
//	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
//	if (EE_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
//	{
//		printf("����eeprom����\r\n");
//		return;
//	}
//	else
//	{
//		printf("����eeprom�ɹ���\r\n");
//	}
//}


///*--------------------------------------------------------------------------------------------------*/
//static void EE_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
//{
//	for(; nCount != 0; nCount--);
//}


///*
// * eeprom AT24C02 ��д����
// * ��������1���쳣����0
// */
//uint8_t EE_Test(void) 
//{
//  uint16_t i;
//	uint8_t write_buf[EEPROM_SIZE];
//  uint8_t read_buf[EEPROM_SIZE];
//  
///*-----------------------------------------------------------------------------------*/  
//  if (EE_CheckOk() == 0)
//	{
//		/* û�м�⵽EEPROM */
//		printf("û�м�⵽����EEPROM!\r\n");
//				
//		return 0;
//	}
///*------------------------------------------------------------------------------------*/  
//  /* �����Ի����� */
//	for (i = 0; i < EEPROM_SIZE; i++)
//	{		
//		write_buf[i] = i;
//	}
///*------------------------------------------------------------------------------------*/  
//  if (EE_WriteBytes(write_buf, 0, EEPROM_SIZE) == 0)
//	{
//		printf("дeeprom����\r\n");
//		return 0;
//	}
//	else
//	{		
//		printf("дeeprom�ɹ���\r\n");
//	}
//  
//  /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
//  EE_Delay(0x0FFFFF);
///*-----------------------------------------------------------------------------------*/
//  if (EE_ReadBytes(read_buf, 0, EEPROM_SIZE) == 0)
//	{
//		printf("��eeprom����\r\n");
//		return 0;
//	}
//	else
//	{		
//		printf("��eeprom�ɹ����������£�\r\n");
//	}
///*-----------------------------------------------------------------------------------*/  
//  for (i = 0; i < EEPROM_SIZE; i++)
//	{
//		if(read_buf[i] != write_buf[i])
//		{
//			printf("0x%02X ", read_buf[i]);
//			printf("����:EEPROM������д������ݲ�һ��");
//			return 0;
//		}
//    printf(" %02X", read_buf[i]);
//		
//		if ((i & 15) == 15)
//		{
//			printf("\r\n");	
//		}		
//	}
//  printf("eeprom��д���Գɹ�\r\n");
//  return 1;
//}

/*
================================================================================================================================================
=======================================HDC1080 FUCTIONs==================================================================================================
*/
uint8_t InitHDC1080_sw_iic(void){
	uint16_t setValue;
	uint16_t humidity, temperature;
	
	//SW_I2C_Config();/*GPIO init*/
	
	setValue = (1<<12); /*������ʪ��һ���ȡģʽ���ֱ��ʶ�Ϊ14λ*/
	/* 1, Configure the acquisiton parameters in register 0x02*/
	SW_I2C_Start();
	SW_I2C_SendByte(ADDRESS_HDC1080 | SW_I2C_WR);	
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
	}	
	SW_I2C_SendByte(ADDRESS_CONFIGURATION);
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
	}	
	SW_I2C_SendByte((uint8_t)(setValue>>8));
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
	}	
	SW_I2C_SendByte((uint8_t)(setValue));
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
	}	
	SW_I2C_Stop();	
	/* 2, Trigger the measurements, executing a point write to 0x00*/
	SW_I2C_Start();
	SW_I2C_SendByte(ADDRESS_HDC1080 | SW_I2C_WR);	
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
	}	
	SW_I2C_SendByte(ADDRESS_TEMPERATURE);	
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
	}	
	
	/* 3, Wait for the measurement to complete*/
	//DelayMs(&operationTime, 20);/*14 bit resolution config correspding 6.5ms of conversion time*/
	delay_ms(20);
	/* 4, Read the temperature and humidity data*/
	SW_I2C_Start();
	SW_I2C_SendByte(ADDRESS_HDC1080 | SW_I2C_RD);	
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
	}	
	temperature = (uint16_t)SW_I2C_ReadByte()<<8;
	I2C_Ack();
	temperature += SW_I2C_ReadByte();
	I2C_Ack();
	humidity = (uint16_t)SW_I2C_ReadByte()<<8;
	I2C_Ack();
	humidity += SW_I2C_ReadByte();
	SW_I2C_NAck();
	SW_I2C_Stop();
	return 0;
}

uint8_t ReadHDC1080_sw_iic(uint16_t *temperature, uint16_t *humidity){
	/* 2, Trigger the measurements, executing a point write to 0x00*/
	SW_I2C_Start();
	SW_I2C_SendByte(ADDRESS_HDC1080 | SW_I2C_WR);	
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
		
		SW_I2C_Stop();
		//printf("1080 ERROR\r\n");
		return 1;
	}
	SW_I2C_SendByte(ADDRESS_TEMPERATURE);	
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
		SW_I2C_Stop();
		//printf("1080 ERROR\r\n");
		return 1;
	}
		
	/* 3, Wait for the measurement to complete*/
	//DelayMs(&operationTime, 20);/*14 bit resolution config correspding 6.5ms of conversion time*/
	delay_ms(20);
	/* 4, Read the temperature and humidity data*/
	SW_I2C_Start();
	SW_I2C_SendByte(ADDRESS_HDC1080 | SW_I2C_RD);	
	if (SW_I2C_WaitAck() != 0)
	{
		HDC1080_CallBack(ERROR_HDC1080_NO_ACK);	/* EEPROM������Ӧ�� */
		SW_I2C_Stop();
		//printf("1080 ERROR\r\n");
		return 1;
	}	
	*temperature = (uint16_t)SW_I2C_ReadByte()<<8;
	I2C_Ack();
	*temperature += SW_I2C_ReadByte();
	I2C_Ack();
	*humidity = (uint16_t)SW_I2C_ReadByte()<<8;
	I2C_Ack();
	*humidity += SW_I2C_ReadByte();
	SW_I2C_NAck();
	SW_I2C_Stop();
	return 0;
}

static void HDC1080_CallBack(uint8_t code){

	switch(code){
		case ERROR_HDC1080_NO_ACK:
			//printf("HDC1080 ERROR_NO_ACK.\r\n");
			break;
		default:
			break;
	}
}




