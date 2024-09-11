#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

uint8_t Read_Crash[256];
uint16_t GSize;

void W25Q64_Init(void)
{
	MySPI_Init();
}

void W25Q64_DMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	GSize = Size;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_BufferSize = Size;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
}

void W25Q64_DMA_SetTransfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, GSize);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(JEDEC_ID);
	*MID = MySPI_SwapByte(Dummy);
	*DID = MySPI_SwapByte(Dummy);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(Dummy);
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(Write_Enable);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint16_t TimeOut = 10000;
	MySPI_Start();
	MySPI_SwapByte(Read_Status_Register_1);
	while((MySPI_SwapByte(Dummy) & 0x01) == 1)
	{
		TimeOut --;
		if(TimeOut == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArry, uint16_t Length)
{
	uint32_t i;
	
	W25Q64_WaitBusy();
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(Page_Program);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0; i < Length; i ++)
	{
		MySPI_SwapByte(DataArry[i]);
	}	
	MySPI_Stop();
	
}

void W25Q64_WriteByte(uint32_t Address, uint8_t Byte)
{
	W25Q64_WaitBusy();
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(Page_Program);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_SwapByte(Byte);
	MySPI_Stop();

}

void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(Sector_Erase_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArry, uint32_t Length)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(Read_Data);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0; i < Length; i ++)
	{
		DataArry[i] = MySPI_SwapByte(Dummy);
	}
	MySPI_Stop();
}

uint8_t W25Q64_ReadByte(uint32_t Address)
{
	uint8_t Byte = 0x00;
	MySPI_Start();
	MySPI_SwapByte(Read_Data);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	Byte = MySPI_SwapByte(Dummy);
	MySPI_Stop();
	return Byte;
}

void W25Q64_SavePhoto(uint32_t Address, uint8_t DataArry[8][128])
{
	uint32_t i, j, k;
	
	for(k = 0; k < 4; k ++)
	{
		W25Q64_WriteEnable();
		
		MySPI_Start();
		MySPI_SwapByte(Page_Program);
		MySPI_SwapByte(Address >> 16);
		MySPI_SwapByte(Address >> 8);
		MySPI_SwapByte(Address);
		for(i = 0; i < 2; i ++)
		{
			for(j = 0; j < 128; j ++)
			{
				MySPI_SwapByte(DataArry[i + 2 * k][j]);
			}
		}
		MySPI_Stop();
		
		W25Q64_WaitBusy();
		Address += 0x000100;
	}
}

void W25Q64_Chip_Erase(void)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(Chip_Erase);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
