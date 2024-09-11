#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Delay.h"

void Key_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启复用IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//关闭SWJ和JTAG_DP使用IO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Key_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
/**@brief		获取按键键值函数
  *@param		无
  *@retval	KeyNum：0~4	分别代表无按键按下，K1按键按下，K2按键按下，K3按键按下，K4按键按下
  */
uint8_t Key_Getnum()
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOA,K1)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA,K1)==0);
		Delay_ms(20);
		KeyNum=1;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,K2)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA,K2)==0);
		Delay_ms(20);
		KeyNum=2;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,K3)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA,K3)==0);
		Delay_ms(20);
		KeyNum=3;
	}
	else if(GPIO_ReadInputDataBit(GPIOA,K4)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA,K4)==0);
		Delay_ms(20);
		KeyNum=4;
	}	
	return KeyNum;
}
