#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "PCF8951.h"
#include "MyAD.h"
#include "W25Q64.h"
#include "Delay.h"

FlagStatus AD_GetFlag = RESET, Load_Flag = RESET;
uint8_t KeyNum;
uint32_t Address_Write = 0x000000, Address_Read = 0x000000;

int main(void)
{
	MyAD_Init();
	OLED_Init();
	Key_Init();
	W25Q64_Init();
	PCF8591_HardWareI2C_Init();
	
//	W25Q64_Chip_Erase();
	
	while(1)
	{
		KeyNum = Key_Getnum();
		if(KeyNum == 1)
		{
			OLED_Clear();
			OLED_ShowString(1, 1, "Writing...");
			AD_GetFlag = !AD_GetFlag;
		}
		if(KeyNum == 2)
		{
			PCF8591_HardWareI2C_DAC_Start();
			OLED_Clear();
			OLED_ShowString(1, 1, "Playing...");
			Load_Flag = SET;
		}
		if(KeyNum == 4)
		{
			OLED_Clear();
			W25Q64_Chip_Erase();
			Address_Write = 0x000000;
			Address_Read = 0x000000;
			OLED_ShowString(1, 1, "Delete OK!");
		}
		while(AD_GetFlag == SET)
		{
			KeyNum = Key_Getnum();
			W25Q64_WriteByte(Address_Write, AD_Value / 16);
			Address_Write ++;
			if(Address_Write == 0x800000 || KeyNum == 3)
			{
				Address_Write = 0;
				AD_GetFlag = RESET;
				OLED_Clear();
				OLED_ShowString(1, 1, "Write OK!");
			}
		}
		while(Load_Flag == SET)
		{
			KeyNum = Key_Getnum();
			PCF8591_HardWareI2C_DAC_Opt(W25Q64_ReadByte(Address_Read));
			Address_Read ++;
			Delay_us(60);
			if(Address_Read == 0x800000 || KeyNum == 3)
			{
				Load_Flag = RESET;
				Address_Read = 0;
				OLED_Clear();
				OLED_ShowString(1, 1, "Play OK!");
			}
		}
	}
}
