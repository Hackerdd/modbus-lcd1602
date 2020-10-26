
#include "1602_iic_sw.h"
//#include "i2c.h"
#include "stm8s_i2c.h"

extern void Delay(__IO uint32_t nTime);

const uint8_t Ainit[42] = {
	0x30,
	0x34,
	0x30,
	0x30,
	0x34,
	0x30,
	0x30,
	0x34,
	0x30,
	0x20,
	0x24,
	0x20,
	0x20,
	0x24,
	0x20,
	0x80,
	0x84,
	0x80,
	0x00,
	0x04,
	0x00,
	0xc0,
	0xc4,
	0xc0,
	0x00,
	0x04,
	0x00,
	0x10,
	0x14,
	0x10,
	0x00,
	0x04,
	0x00,
	0x60,
	0x64,
	0x60,
	0x00,
	0x04,
	0x00,
	0x20,
	0x24,
	0x20,
};

//set default status of backlight to ON
static uint8_t backLight = BACKLIGHT_ON;

void iic_write(uint8_t addr, uint8_t data)
{
	uint16_t time_over = 0;
	while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
	{
		time_over++;
		if (time_over > 0x100)
		{
			time_over = 0;
			return;
		}
	}
	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
	{
		time_over++;
		if (time_over > 0x100)
		{
			time_over = 0;
			return;
		}
	}
	I2C_Send7bitAddress((uint8_t)(addr), I2C_DIRECTION_TX);
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		time_over++;
		if (time_over > 0x100)
		{
			time_over = 0;
			return;
		}
	} // I2C_SendData((uint8_t)(0x08));
	I2C_SendData((uint8_t)(data));
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		time_over++;
		if (time_over > 0x100)
		{
			time_over = 0;
			return;
		}
	}
	while (!I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED))
	{
		time_over++;
		if (time_over > 0x100)
		{
			time_over = 0;
			return;
		}
	}
	I2C_GenerateSTOP(ENABLE);
}

/***
	initial 1602 pins and mode
***/
void init_1602(void)
{
	uint8_t i = 0;
	//	IIC_Init();
	i2cAction(0);
	Delay(500);

	for (i = 0; i < 42; ++i)
	{
		i2cAction(Ainit[i]);

		if ((i == 2) || (i == 5))
			Delay(5);
		else if ((i == 29) || (i == 41))
			Delay(2);
		else if (i == 8)
			Delay(1);
		else
			Delay(1);
	}
}

/***
	Output charactors on 1602
***/
void lcdPrint(const char *charactor, unsigned char len)
{
	uint8_t c = 0;
	for (c = 0; c < len; ++c)
		writeOneChar(DATA, *(charactor + c));
}

void disOneChar(unsigned char X, unsigned char Y, char DData)
{
	Y &= 0x1;
	X &= 0xF; //限制 X 不能大于 15，Y 不能大于 1
	if (Y)
		X |= 0x40;		  //当要显示第二行时地址码+0x40;
	X |= 0x80;			  //算出指令码
	writeOneChar(CMD, X); //这里不检测忙信号，发送地址码
	writeOneChar(DATA, DData);
}

void writeOneChar(const uint8_t cmdOrData, const uint8_t Data)
{
	uint8_t LCM_Data = 0;
	LCM_Data = ((Data & 0xF0) | (cmdOrData) | (backLight));
	i2cAction(LCM_Data);

	i2cAction(LCM_Data | 0x04);
	Delay(1); //20us better

	LCM_Data = (((Data & 0x0F) << 4) | (cmdOrData) | (backLight));
	i2cAction(LCM_Data);

	i2cAction(LCM_Data | 0x04);
	Delay(1); //20us better
	i2cAction(LCM_Data);
}

/***
	set position of cursor
***/
void setCursor(uint8_t colmn, uint8_t line)
{
	line &= 0x1;
	colmn &= 0xF; //限制 X 不能大于 15，line 不能大于 1
	if (line)
		colmn |= 0x40;		  //当要显示第二行时地址码+0x40;
	colmn |= 0x80;			  //算出指令码
	writeOneChar(CMD, colmn); //这里不检测忙信号，发送地址码
}

/***
	send one frame by iic
***/
void i2cAction(uint8_t status)
{
	//	IIC_Start();
	//	IIC_Send_Byte(ADDRWRITE);
	//	IIC_Wait_Ack();
	//	IIC_Send_Byte(status);
	//	IIC_Wait_Ack();
	//	IIC_Stop();

	iic_write((ADDRWRITE), status);
}
