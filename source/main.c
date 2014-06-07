/**
 * Quadrocopter STM32 main.c
 * 2014
 */

#include "bluetooth.h"

int main(void)
{
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  init_bluetooth();

  
  while(1)
  {
    /* Loop and wait for interrupts */
    int i = 10000000;
    while(i--); 
    USART_SendData(USART3, 0x74);
  }
}

