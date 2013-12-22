/***************************************
 * Quadrocopter STM32 main.c
 * 2013-10-11
***************************************/

#include "stm32f30x.h"
#include "stm32f3_discovery.h"

int main(void)
{
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDToggle(LED4);

  /* Initialize GPIO PIN */
  /* GPIOC_Init(); */
  /* Initialize USART3 */
  /* USART3_Init(); */
  
  /* Start by sending first character */
  /* USART_SendData(USART3, 't'); */

  while(1)
  {
    /* Loop and wait for interrupts */
  }
}

/*********************************************
 * Sample - LED toggle
 ********************************************/
void led_toggle(void)
{
  uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7);

  STM_EVAL_LEDToggle(LED3);
  /* if(led_bit == (uint8_t)Bit_SET) */
  /* { */
  /*   GPIO_ResetBits(GPIOC, GPIO_Pin_7); */
  /* } */
  /* else */
  /* { */
  /*   GPIO_SetBits(GPIOC, GPIO_Pin_7); */
  /* } */
}

