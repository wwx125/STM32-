#ifndef __MyUSART_H
#define __MyUSART_H

#include <stdio.h>

extern uint8_t MyUSART_TxPacket[4];
extern char MyUSART_RxPacket[100];
extern uint8_t MyUSART_RxPacketFlag;

void MyUSART_Init(void);
void MyUSART_SendByte(uint8_t Byte);
void MyUSART_SendArray(uint8_t *Array, uint16_t Length);
void MyUSART_SendString(char *String);
void MyUSART_SendNum(uint32_t Number, uint16_t Length);
void MyUSART_Printf(char *format, ...);
void MyUSART_SendHexPacket(void);

uint8_t MyUSART_ReceiveFlag(void);

#endif
