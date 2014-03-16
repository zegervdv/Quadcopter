/**
 * stm32f30x_it.c: Manage interrupt handlers
 */

#include "stm32f3_discovery.h"

int i;


void EXTI0_IRQHandler(void)
{
  if ((EXTI_GetITStatus(USER_BUTTON_EXTI_LINE) == SET)&&(STM_EVAL_PBGetState(BUTTON_USER) != RESET))
  {
    /* Delay */
    for(i=0; i<0x7FFFF; i++);

    while(STM_EVAL_PBGetState(BUTTON_USER) != RESET);

    for(i=0; i<0x7FFFF; i++);

    USART_SendData(USART3, 't');
    STM_EVAL_LEDToggle(LED4);
  }
}
