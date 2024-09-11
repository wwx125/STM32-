#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t ReceiveFlag;
uint8_t MyUSART_TxPacket[4];
char MyUSART_RxPacket[100];
uint8_t MyUSART_RxPacketFlag;

void MyUSART_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//设置波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送模式
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);	
}

void MyUSART_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待发送标志位置1
}

void MyUSART_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		MyUSART_SendByte(Array[i]);
	}
}

void MyUSART_SendString(char *String)
{
	uint16_t i;
	for(i = 0; String[i] != '\0'; i++)
	{
		MyUSART_SendByte(String[i]);
	}
}

uint16_t MyUSART_Pow(uint16_t X, uint16_t Y)
{
	uint32_t Result = 1;
	while(Y --)
	{
		Result *= X;
	}
	return Result;
}

void MyUSART_SendNum(uint32_t Number, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		MyUSART_SendByte(Number / MyUSART_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**@brief	重定向printf函数使其输出到串口1	
  *@param	
  *@retval	
  */
int fputc(int ch, FILE *f)
{
	MyUSART_SendByte(ch);
	return ch;
}

void MyUSART_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	MyUSART_SendString(String);
}

void MyUSART_SendHexPacket(void)
{
	MyUSART_SendByte(0xFF);
	MyUSART_SendArray(MyUSART_TxPacket, 4);
	MyUSART_SendByte(0xFE);
}

uint8_t MyUSART_ReceiveFlag(void)
{
	if(ReceiveFlag == 1)
	{
		ReceiveFlag = 0;
		return 1;
	}
	return 0;
}

void USART1_IRQHandler(void)
{
	static uint8_t Status = 0;//状态机标志位，为0检查包头，为1接收数据，为2检查包尾
	static uint8_t DataNum = 0;//数据接收标志位
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		if(Status == 0)
		{
			if(USART_ReceiveData(USART1) == '@')
			{
				Status = 1;
				DataNum = 0;
			}
		}
		else if(Status == 1)
		{
			if(USART_ReceiveData(USART1) == '\r')
			{
				Status = 2;
			}
			else
			{
				MyUSART_RxPacket[DataNum] = USART_ReceiveData(USART1);
				DataNum ++;
			}
		}
		else if(Status == 2)
		{
			if(USART_ReceiveData(USART1) == '\n')
			{
				Status = 0;
				MyUSART_RxPacket[DataNum] = '\0';
				MyUSART_RxPacketFlag = 1;//接收数据包完成标志位，需手动置0
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
