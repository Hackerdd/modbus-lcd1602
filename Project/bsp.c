

#include "bsp.h"
#include "1602_iic_sw.h"

// CRC 高位字节值表
static const uint8_t s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40};
// CRC 低位字节值表
const uint8_t s_CRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40};

/*
*********************************************************************************************************
*	函 数 名: CRC16_Modbus
*	功能说明: 计算CRC。 用于Modbus协议。
*	形    参: _pBuf : 参与校验的数据
*			  _usLen : 数据长度
*	返 回 值: 16位整数值。 对于Modbus ，此结果高字节先传送，低字节后传送。
*
*   所有可能的CRC值都被预装在两个数组当中，当计算报文内容时可以简单的索引即可；
*   一个数组包含有16位CRC域的所有256个可能的高位字节，另一个数组含有低位字节的值；
*   这种索引访问CRC的方式提供了比对报文缓冲区的每一个新字符都计算新的CRC更快的方法；
*
*  注意：此程序内部执行高/低CRC字节的交换。此函数返回的是已经经过交换的CRC值；也就是说，该函数的返回值可以直接放置
*        于报文用于发送；
*********************************************************************************************************
*/
uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucCRCHi = 0xFF; /* 高CRC字节初始化 */
    uint8_t ucCRCLo = 0xFF; /* 低CRC 字节初始化 */
    uint16_t usIndex;       /* CRC循环中的索引 */

    while (_usLen--)
    {
        usIndex = ucCRCHi ^ *_pBuf++; /* 计算CRC */
        ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
        ucCRCLo = s_CRCLo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}

/*
*********************************************************************************************************
*	函 数 名: BEBufToUint16
*	功能说明: 将2字节数组(大端Big Endian次序，高字节在前)转换为16位整数
*	形    参: _pBuf : 数组
*	返 回 值: 16位整数值
*
*   大端(Big Endian)与小端(Little Endian)
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}

uint8_t modbus_app(uint8_t com_cmds[], uint16_t _usLen, Mb_CMD_Typedef *mbCmd)
{
    uint16_t crc_value = 0;
    crc_value = com_cmds[_usLen - 2] * 256 + com_cmds[_usLen - 1];
    if (CRC16_Modbus(com_cmds, _usLen - 2) == crc_value)
    {
        if (com_cmds[0] == pMb->mb_addr)
        {
            if (com_cmds[1] == 0x06)
            {
                /* 01 06 00 02 [00] [30] 28 1E  */
                /*写单寄存器*/
                if (BEBufToUint16(&com_cmds[2]) < 0x20)
                {
                    /* 应答 */
                    uart_send(com_cmds, _usLen);
                    // uint8_t x, y;
                    // x = com_cmds[3] % 16;
                    // y = com_cmds[3] / 16;
                    pMb->lcd[com_cmds[3]] = com_cmds[5];
                    mbCmd->cmd = CMD_LCD;
                    mbCmd->len = 1;
                    mbCmd->mptr = com_cmds[3];
                    return 0;
                }
                else if (BEBufToUint16(&com_cmds[2]) == BK_LIGHT_REG_ADDR)
                {
                    uart_send(com_cmds, _usLen);
                    pMb->bk_light = com_cmds[5];
                    mbCmd->cmd = CMD_BK_LIGHT;
                    mbCmd->len = 1;
                    mbCmd->mptr = com_cmds[3];
                    return 0;
                }
                else if (BEBufToUint16(&com_cmds[2]) == MB_ADDR_REG_ADDR)
                {
                    uart_send(com_cmds, _usLen);
                    pMb->mb_addr = com_cmds[5];
                    mbCmd->cmd = CMD_MB_ADDR;
                    mbCmd->len = 1;
                    mbCmd->mptr = com_cmds[3];
                    return 0;
                }
                return 1;
            }
            else if (com_cmds[1] == 0x10)
            {
                if (BEBufToUint16(&com_cmds[2]) < 0x32)
                {
                    uint8_t response_ack[8] = {0};
//                    response_ack[0] = com_cmds[0];
//                    response_ack[1] = com_cmds[1];
//                    response_ack[2] = com_cmds[2];
//                    response_ack[3] = com_cmds[3];
//                    response_ack[4] = com_cmds[4];
//                    response_ack[5] = com_cmds[5];
//                    response_ack[6] = CRC16_Modbus(response_ack, 6) >> 8;
//                    response_ack[7] = CRC16_Modbus(response_ack, 6) & 0x00ff;
                    
//                    response_ack[0] = com_cmds[0];
//                    response_ack[1] = com_cmds[1];
//                    response_ack[2] = com_cmds[2];
//                    response_ack[3] = com_cmds[3];
//                    response_ack[4] = com_cmds[4];
//                    response_ack[5] = com_cmds[5];
                    response_ack[0] = CRC16_Modbus(com_cmds, 6) >> 8;
                    response_ack[1] = CRC16_Modbus(com_cmds, 6) & 0x00ff;
                    uart_send(com_cmds, 6);
                    uart_send(response_ack, 2);

                    for (uint8_t i = 0; i < com_cmds[5]; i++)
                    {
                        /*com_cmds[3]起始地址低字节*/
                        pMb->lcd[com_cmds[3]+i] = com_cmds[8+i*2];
                    }
                    
                    mbCmd->cmd = CMD_LCD;
                    mbCmd->len = com_cmds[5];
                    mbCmd->mptr = com_cmds[3];
                    return 0;

                    return 0;
                }
                return 1;
            }
            else if (com_cmds[1] == 0x03)
            {
                /* 01 03 00 00 00 14 45 C5 */
                if (BEBufToUint16(&com_cmds[2]) < MB_SIZE)
                {
                    uint8_t _h03_ack[MB_SIZE*2+10] = {0};
                    uint16_t CRC = 0;
                    _h03_ack[0] = pMb->mb_addr;
                    _h03_ack[1] = 0x03;
                    _h03_ack[2] = com_cmds[5] * 2;
                    uint16_t i = 0;
                    for (i = 0; i < com_cmds[5]; i++)
                    {
                        _h03_ack[4 + 2 * i] = g_modbus_buffer[com_cmds[3] + i];
                    }
                    CRC = CRC16_Modbus(_h03_ack, 4 + (2 * (i-1)) + 1);
                    _h03_ack[4 + 2 * (i-1) + 1] = CRC>>8;
                    _h03_ack[4 + 2 * (i-1) + 2] = CRC&0x00ff;
                    uart_send(_h03_ack, 4 + 2 * (i-1) + 3);
                }
            }
            return 1;
        }
        return 1;
    }
}

void led_init(void)
{
    /* LED引脚初始化 */
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
    /* 485收发引脚初始化 ，初始化为高电平，接收模式*/
    GPIO_Init(RS485_PORT, RS485_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void myiic_init(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    /*!< GPIO configuration */
    /*!< Configure sEE_I2C pins: SCL */
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST);

    /*!< Configure sEE_I2C pins: SDA */
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);

    I2C_Cmd(ENABLE);

    I2C_Init(I2C_MAX_STANDARD_FREQ, 0x01, I2C_DUTYCYCLE_2, I2C_ACK_CURR,
             I2C_ADDMODE_7BIT, 16);

    /* Perform a read on SR1 and SR3 register to clear eventually pending flags */
    (void)I2C->SR1;
    (void)I2C->SR3;
}

void my_uart_init(void)
{
    UART1_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);

    UART1_Init(19200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,
               UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    UART1_Cmd(ENABLE);

    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART1_ITConfig(UART1_IT_IDLE, ENABLE);

    __enable_interrupt();
}

void uart_send(uint8_t *ptr, uint16_t len)
{
    RS485_SEND_EN();
    for (uint16_t i = 0; i < len; i++)
    {
        /* Write a character to the UART1 */
        UART1_SendData8(*(ptr + i));
        /* Loop until the end of transmission */
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            ;
    }
    while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET);
    
    RS485_RECEIVE_EN();
}

void bsp_init(void)
{
    led_init();
    myiic_init();
    my_uart_init();
}
