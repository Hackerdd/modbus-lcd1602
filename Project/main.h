/***
	Copyright (c) 2017
	All rights reserved.

	File name           :main.h
	file identification :
	Subject             :Say something about the file

	Current Version     :V1.0.0
	Author              :Hacker
	Date                :

	Instead Version     :
	Author              :Hacker
	Date                :
***/
#ifndef __MAIN_H
#define __MAIN_H value

#include "stm8s_gpio.h"
#include "stm8s.h"
#include "stm8s_clk.h"

#define MB_SIZE 0x30

typedef struct
{
	uint8_t lcd[32];
	uint8_t bk_light;
	uint8_t mb_addr;
}Mb_Struct_Typedef;

#define BK_LIGHT_REG_ADDR (0x20)
#define MB_ADDR_REG_ADDR (0x21)

extern uint8_t g_modbus_buffer[MB_SIZE];
extern Mb_Struct_Typedef *pMb;

typedef enum
{
	CMD_LCD = 1,
	CMD_BK_LIGHT,
	CMD_MB_ADDR,
}CMD_Typedef_Enum;

typedef struct
{
	uint8_t mptr;
	CMD_Typedef_Enum cmd;
	uint8_t len;
}Mb_CMD_Typedef;

#endif