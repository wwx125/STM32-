#ifndef __W25Q64_INS_H
#define __W25Q64_INS_H

#define Write_Enable 0x06
#define Write_Disable 0x04
#define Read_Status_Register_1 0x05
#define Read_Status_Register_2 0x35
#define Write_Status_Register 0x01
#define Page_Program 0x02
#define Quad_Page_Program 0x32
#define Block_Erase_64KB 0xD8
#define Block_Erase_32KB 0x52
#define Sector_Erase_4KB 0x20
#define Chip_Erase 0xC7
#define Erase_Suspend 0x75
#define Erase_Resume 0x7A
#define Power_down 0xB9
#define High_Performance_Mode 0xA3
#define Continuous_Read_Mode_Reset 0xFF
#define Release_Power_down_or_HPM 0xAB
#define Manufacturer 0x90
#define Read_Unique_ID 0x4B
#define JEDEC_ID 0x9F
#define Read_Data 0x03
#define Dummy 0xFF

#endif
