#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArry, uint16_t Length);
void W25Q64_WriteByte(uint32_t Address, uint8_t Byte);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArry, uint32_t Length);
uint8_t W25Q64_ReadByte(uint32_t Address);
void W25Q64_SavePhoto(uint32_t Address, uint8_t DataArry[8][128]);
void W25Q64_Chip_Erase(void);

void W25Q64_DMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size);
void W25Q64_DMA_SetTransfer(void);

#endif
