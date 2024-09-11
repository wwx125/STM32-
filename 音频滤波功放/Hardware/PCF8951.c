#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"

#define PCF8591_Address_Write	0x90
#define PCF8591_Address_Read	0x91

void PCF8591_SoftWareI2C_Init(void)
{
	MyI2C_Init();
}

void PCF8591_SoftWareI2C_ADC_Start(uint8_t Command)
{
	MyI2C_Start();
	MyI2C_SendByte(PCF8591_Address_Write);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Command);
	MyI2C_Start();
	MyI2C_SendByte(PCF8591_Address_Read);
	MyI2C_ReceiveAck();
}

uint8_t PCF8591_SoftWareI2C_ADC_Ipt(uint8_t Command)
{
	uint8_t Data = 0;
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	return Data;
}

void PCF8591_SoftWareI2C_ADC_Stop(void)
{
	MyI2C_Stop();
}

void PCF8591_SoftWareI2C_DAC_Start(void)
{
	MyI2C_Start();
	MyI2C_SendByte(PCF8591_Address_Write);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0x40);
	MyI2C_ReceiveAck();
}

void PCF8591_SoftWareI2C_DAC_Opt(uint8_t Value)
{
	MyI2C_SendByte(Value);
	MyI2C_ReceiveAck();
}

void PCF8591_SoftWareI2C_DAC_Stop(void)
{
	MyI2C_Stop();
}

void I2C_EventWait(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint16_t LOOP = 10000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		LOOP --;
		if(LOOP == 0)
			break;
	}
}

void PCF8591_HardWareI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
}

void PCF8591_HardWareI2C_ADC_Start(uint8_t Command)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, PCF8591_Address_Write, I2C_Direction_Transmitter);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, Command);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, PCF8591_Address_Write, I2C_Direction_Receiver);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
}

uint8_t PCF8591_HardWareI2C_ADC_Ipt(void)
{
	uint8_t Data = 0;
	
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C2);
	
	return Data;
}

void PCF8591_HardWareI2C_ADC_Stop(void)
{
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
}

void PCF8591_HardWareI2C_DAC_Start(void)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, PCF8591_Address_Write, I2C_Direction_Transmitter);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, 0x40);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
}

void PCF8591_HardWareI2C_DAC_Opt(uint8_t Value)
{
	I2C_SendData(I2C2, Value);
	I2C_EventWait(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
}

void PCF8591_HardWareI2C_DAC_Stop(void)
{
	I2C_GenerateSTOP(I2C2, ENABLE);
}
