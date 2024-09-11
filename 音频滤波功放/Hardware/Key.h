#ifndef __KEY_H
#define __KEY_H

#define Key_GPIO_Pin GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define K1 GPIO_Pin_15
#define K2 GPIO_Pin_14
#define K3 GPIO_Pin_13
#define K4 GPIO_Pin_12

void Key_Init(void);
uint8_t Key_Getnum(void);

#endif
