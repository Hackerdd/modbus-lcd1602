/***
	Copyright (c) 2017
	All rights reserved.

	File name           :main.c
	file identification :
	Subject             :Say something about the file

	Current Version     :V1.0.0
	Author              :Hacker
	Date                :

	Instead Version     :
	Author              :Hacker
	Date                :
***/
#include "main.h"
#include "1602_iic_sw.h"
#include "bsp.h"

static uint8_t rx_data[50] = {0};
static uint8_t rx_index = 0;
static uint8_t rx_done_flag = 0;

uint8_t g_modbus_buffer[MB_SIZE] = {0};
Mb_Struct_Typedef *pMb = (Mb_Struct_Typedef *)g_modbus_buffer;

/**
  * @addtogroup TIM4_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM4_PERIOD 124
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
static void CLK_Config(void);
static void TIM4_Config(void);

int main()
{
  uint8_t mbSlaveaddr = 0x01;
  pMb->mb_addr = mbSlaveaddr;
  pMb->bk_light = 1;
  CLK_Config();
  TIM4_Config();
  bsp_init();

  init_1602();
  i2cAction(BACKLIGHT_ON);

  setCursor(0, 0);
  uint8_t data[] = "MB ADDR:[0x01]";
  data[11] = (pMb->mb_addr / 0x10) + '0';
  data[12] = (pMb->mb_addr % 0x10) + '0';
  lcdPrint((char*)data, sizeof(data) - 1);

  Delay(500);

  setCursor(0, 0);
  for (uint8_t i = 0; i < 16; i++)
  {
    writeOneChar(DATA, ' ');
  }

  while (1)
  {
    uint8_t status = 0;
    static uint32_t tick = 0;
    static Mb_CMD_Typedef mbCMD;
    GPIO_WriteReverse(GPIOD, GPIO_PIN_3);

    tick = TimingDelay;
    while (TimingDelay - tick < 500)
    {
      if (rx_done_flag != 0)
      {
        status = modbus_app(rx_data, rx_index, &mbCMD);
        if (status == 0)
        {
          switch (mbCMD.cmd)
          {
          case CMD_LCD:
            /* code */
            {
                            uint8_t x;
            uint8_t y;
            x = mbCMD.mptr % 16;
            y = mbCMD.mptr / 16;

            for (uint8_t i = 0; i < mbCMD.len; i++)
            {
              if (x + i >= 0x10)
              {
                y = 1;
              }
              setCursor(x + i, y);
              writeOneChar(DATA, pMb->lcd[mbCMD.mptr + i]);
            }
            }

            break;
          case CMD_BK_LIGHT:
            if (pMb->lcd[mbCMD.mptr] == 1)
              i2cAction(BACKLIGHT_ON);
            else
              i2cAction(BACKLIGHT_OFF);
            break;
          case CMD_MB_ADDR:
            /* code */
            {
                            setCursor(0, 0);
            uint8_t datax[] = "MB ADDR:[0x01]";
            datax[11] = (pMb->mb_addr / 0x10) + '0';
            datax[12] = (pMb->mb_addr % 0x10) + '0';
            lcdPrint((char*)datax, sizeof(datax) - 1);

            }
            break;

          default:
            break;
          }
        }

        rx_index = 0;
        rx_done_flag = 0;
      }
    }
  }

  return 0;
}

/**
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Initialization of the clock */
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);

  /* enable interrupts */
  enableInterrupts();

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(uint32_t nTime)
{
  uint32_t _tick = 0;
  _tick = TimingDelay;

  while (TimingDelay - _tick < nTime)
    ;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  //  if (TimingDelay != 0x00)
  //  {
  //    TimingDelay--;
  //  }

  TimingDelay++;
}

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

  if (UART1_GetITStatus(UART1_IT_RXNE) != RESET)
  {
    rx_data[rx_index++] = UART1->DR;
    //        UART1_ClearITPendingBit(UART1_IT_RXNE);
  }
  if (UART1_GetITStatus(UART1_IT_IDLE) != RESET)
  {
    rx_data[rx_index] = UART1->DR;
    //        uart_send(rx_data, rx_index);
    //        setCursor(0, 0);
    //        lcdPrint(rx_data);
    //        rx_index = 0;
    rx_done_flag = 1;
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */
