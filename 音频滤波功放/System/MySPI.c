#include "stm32f10x.h"                  // Device header

void MySPI_W_CS(BitAction BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, BitValue);
}

void MySPI_W_SCK(BitAction BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, BitValue);
}

void MySPI_W_MOSI(BitAction BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, BitValue);
}

BitAction MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	MySPI_W_CS(1);
	MySPI_W_SCK(0);
}

void MySPI_Start(void)
{
	MySPI_W_CS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_CS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ReceiveData = 0x00;
	for(i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(ByteSend & 0x80);
		ByteSend <<= 1;
		MySPI_W_SCK(1);
		ByteSend |= MySPI_R_MISO();
		MySPI_W_SCK(0);
	}
	return ByteSend;
}
