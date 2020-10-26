#ifndef __BSP_H
#define __BSP_H



#include "main.h"
#include "stm8s_uart1.h"


uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);

uint8_t modbus_app(uint8_t com_cmds[], uint16_t _usLen, Mb_CMD_Typedef* mbCmd);

void uart_send(uint8_t *ptr, uint16_t len);

void bsp_init(void);

#endif // !__BSP_H

