#ifndef __BSP_H
#define __BSP_H

#include "main.h"
#include "stm8s_uart1.h"

#define LED_PORT GPIOD
#define LED_PIN GPIO_PIN_3

#define RS485_PORT GPIOD
#define RS485_PIN GPIO_PIN_4

#define RS485_RECEIVE_EN()  GPIO_WriteHigh(RS485_PORT, RS485_PIN)
#define RS485_SEND_EN()     GPIO_WriteLow(RS485_PORT, RS485_PIN)

uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);

uint8_t modbus_app(uint8_t com_cmds[], uint16_t _usLen, Mb_CMD_Typedef *mbCmd);

void uart_send(uint8_t *ptr, uint16_t len);

void bsp_init(void);

#endif // !__BSP_H
