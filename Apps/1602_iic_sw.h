#ifndef __1602_IIC_SW_H__
#define __1602_IIC_SW_H__

#include "main.h"

#define ADDR	0x27

//High bit to R,Loe bit to W
#define ADDRREAD	((ADDR<<1) | 0x01)
#define ADDRWRITE	(ADDR<<1)

#define BACKLIGHT_ON	0x08
#define BACKLIGHT_OFF	0x00 
#define LCD_CLC	0x00

#define CMD 	0
#define DATA	1

void iic_write(uint8_t addr, uint8_t data);
void init_1602(void);
void lcdPrint(const char* charactor, unsigned char len);
void disOneChar(unsigned char X, unsigned char Y, char DData);
void writeOneChar(const uint8_t cmdOrData, const uint8_t Data);
void setCursor(uint8_t colmn, uint8_t line);
void i2cAction(uint8_t status);

#endif
