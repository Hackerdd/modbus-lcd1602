/***
	Copyright (c) 2017
	All rights reserved.

	File name           :ch455g.h
	file identification :
	Subject             :Say something about the file

	Current Version     :V1.0.0
	Author              :Hacker
	Date                :

	Instead Version     :
	Author              :Hacker
	Date                :
***/
#ifndef __CH455_H
#define __CH455_H

#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"

#define SDA_PIN        GPIO_PIN_5 
#define CLK_PIN        GPIO_PIN_4 

#define IIC_PORT       GPIOB

#define CH455_IO_SDA_GET() GPIO_ReadInputPin(CH455_PORT, SDA_PIN)

void CH455_init(void);
void CH455_Write( unsigned char byte1, unsigned char byte2);
void CH455_Read( unsigned char wByte, unsigned char *rByte );
void CH455_Write_Num(unsigned char seg, unsigned char val, unsigned char dot);
void CH455_Write_Segment(unsigned char seg, unsigned char val);

#endif