#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define GPIO_I2C_Pin GPIO_Pin_10 | GPIO_Pin_11
#define I2C_SCL GPIO_Pin_10
#define I2C_SDA GPIO_Pin_11

void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, I2C_SCL, (BitAction)BitValue);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, I2C_SDA, (BitAction)BitValue);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, I2C_SDA);
	return BitValue;
}

void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出，弱上拉模式
	GPIO_InitStructure.GPIO_Pin = GPIO_I2C_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB, I2C_SCL, Bit_SET);
	GPIO_WriteBit(GPIOB, I2C_SDA, Bit_SET);
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t Byte=0, i;
	MyI2C_W_SDA(1);//释放SDA，控制权给从机					
	for(i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA())
			Byte |= (0x80 >> i);
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t Ack)
{
	MyI2C_W_SDA(Ack);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t Ack = 1;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	Ack = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return Ack;
}
