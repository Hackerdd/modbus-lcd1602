/***
	Copyright (c) 2017
	All rights reserved.

	File name           :iic_soft.c
	file identification :
	Subject             :Say something about the file

	Current Version     :V1.0.0
	Author              :Hacker
	Date                :

	Instead Version     :
	Author              :Hacker
	Date                :
***/
#include "iic.h"

const unsigned char BCD_decode_tab[0x10] = { 0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71 };

static void delay(unsigned int val)
{
	int i = 0;
	for (i = val; i > 0; i--)
	for (int scale = 0; scale < 16; ++scale);
}

void iic_init(void)
{
   GPIO_Init(IIC_PORT,SDA_PIN,GPIO_MODE_OUT_OD_HIZ_FAST );//定义LED的管脚的模式
   GPIO_Init(IIC_PORT,CLK_PIN,GPIO_MODE_OUT_OD_HIZ_FAST );
}

static void SDA_SET_OUTPUT(void)
{
	GPIO_Init(IIC_PORT,SDA_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );//定义LED的管脚的模式
}

static void SDA_SET_INPUT(void)
{
	GPIO_Init(IIC_PORT,SDA_PIN,GPIO_MODE_IN_PU_NO_IT );//定义LED的管脚的模式
}

static void SDA_PIN_SET(int val)
{	
	if(val == 0)GPIO_WriteLow(IIC_PORT, SDA_PIN);//输入低电平
	else GPIO_WriteHigh(IIC_PORT, SDA_PIN);//输入低电平
}

static void CLK_PIN_SET(int val)
{
	if(val == 0)GPIO_WriteLow(IIC_PORT, CLK_PIN);//输入低电平
	else GPIO_WriteHigh(IIC_PORT, CLK_PIN);//输入低电平
}

static void iic_BusStart( void )
{
	SDA_SET_OUTPUT();//iic_IO_SDA_TG_OUTPUT();

	SDA_PIN_SET(1);// iic_IO_SDA_SET_HI();
	CLK_PIN_SET(1);// iic_IO_SCL_SET_HI(); 
	delay(100);// iic_IO_MCU_WAIT_NS(100);
	SDA_PIN_SET(0);// iic_IO_SDA_SET_LO(); 
	delay(100);// iic_IO_MCU_WAIT_NS(100);
	CLK_PIN_SET(0);// iic_IO_SCL_SET_LO();
}

static void iic_BusStop( void )
{
	SDA_SET_OUTPUT();// iic_IO_SDA_TG_OUTPUT(); 

	SDA_PIN_SET(0);// iic_IO_SDA_SET_LO();  
	delay(100);// iic_IO_MCU_WAIT_NS(100);
	CLK_PIN_SET(1);// iic_IO_SCL_SET_HI();
	delay(100);// iic_IO_MCU_WAIT_NS(100);
	SDA_PIN_SET(1);// iic_IO_SDA_SET_HI();
}

static void iic_WriteByte( unsigned char wByte )
{
	unsigned char i = 0;

	SDA_SET_OUTPUT();

	for ( i = 0x80; i >= 0x01; i >>= 1 )
	{
		if ( i & wByte )
			SDA_PIN_SET(1);	//iic_IO_SDA_SET_HI();
		else 
			SDA_PIN_SET(0);	//    iic_IO_SDA_SET_LO();

		delay(100); 
		CLK_PIN_SET(1);//iic_IO_SCL_SET_HI();
		delay(100); 
		CLK_PIN_SET(0);//iic_IO_SCL_SET_LO();    
	}

	//Recieve ACK
	SDA_PIN_SET(1);;//iic_IO_SDA_SET_HI();
	delay(100); 
	CLK_PIN_SET(1);//iic_IO_SCL_SET_HI();
	delay(100); 
	CLK_PIN_SET(0);//iic_IO_SCL_SET_LO(); 
}

void iic_Write( unsigned char byte1, unsigned char byte2)
{    
	iic_BusStart();

	iic_WriteByte( byte1 );
	iic_WriteByte( byte2 );

	iic_BusStop();
}

void iic_Read( unsigned char wByte, unsigned char *rByte )
{
	unsigned char i = 0, readByte = 0;

	iic_BusStart();

	iic_WriteByte( wByte );

	SDA_SET_INPUT();//iic_IO_SDA_TG_INPUT();

	for ( i = 0x80; i >= 0x01; i >>= 1 )
	{
		delay(100); 
		CLK_PIN_SET(1);//iic_IO_SCL_SET_HI();

		if ( iic_IO_SDA_GET() )
		readByte |= i; 

		delay(100); 
		CLK_PIN_SET(0);//iic_IO_SCL_SET_LO();     
	}

	SDA_SET_OUTPUT();//iic_IO_SDA_TG_OUTPUT();
	SDA_PIN_SET(1);;//iic_IO_SDA_SET_HI();
	delay(100);
	SDA_PIN_SET(1);;//iic_IO_SDA_SET_HI();
	delay(100);
	SDA_PIN_SET(0);//iic_IO_SDA_SET_LO();

	iic_BusStop();

	*rByte = readByte;
}

void iic_Write_Num(unsigned char seg, unsigned char val, unsigned char dot)
{
	switch(seg)
	{
		case 0:
    		iic_Write(0x68, BCD_decode_tab[val]|(dot<<7));
    		break;
		case 1:
    		iic_Write(0x6A, BCD_decode_tab[val]|(dot<<7));
    		break;
		case 2:
    		iic_Write(0x6C, BCD_decode_tab[val]|(dot<<7));
    		break;
		case 3:
    		iic_Write(0x6E, BCD_decode_tab[val]|(dot<<7));
    		break;
    	default:
    		break;
	}
}

void iic_Write_Segment(unsigned char seg, unsigned char val)
{
	switch(seg)
	{
		case 0:
    		iic_Write(0x68, val);
    		break;
		case 1:
    		iic_Write(0x6A, val);
    		break;
		case 2:
    		iic_Write(0x6C, val);
    		break;
		case 3:
    		iic_Write(0x6E, val);
    		break;
    	default:
    		break;
	}
}
