#ifndef __PCF8951_H
#define __PCF8951_H

void PCF8591_SoftWareI2C_Init(void);

void PCF8591_SoftWareI2C_ADC_Start(uint8_t Command);
uint8_t PCF8591_SoftWareI2C_ADC_Ipt(uint8_t Command);
void PCF8591_SoftWareI2C_ADC_Stop(void);

void PCF8591_SoftWareI2C_DAC_Start(void);
void PCF8591_SoftWareI2C_DAC_Opt(uint8_t Value);
void PCF8591_SoftWareI2C_DAC_Stop(void);

void PCF8591_HardWareI2C_Init(void);

void PCF8591_HardWareI2C_ADC_Start(uint8_t Command);
uint8_t PCF8591_HardWareI2C_ADC_Ipt(void);
void PCF8591_HardWareI2C_ADC_Stop(void);

void PCF8591_HardWareI2C_DAC_Start(void);
void PCF8591_HardWareI2C_DAC_Opt(uint8_t Value);
void PCF8591_HardWareI2C_DAC_Stop(void);

#endif
