/**
 * stm32f30x_it.c: Manage interrupt handlers
 */

#include "stm32f3_discovery.h"
#include "stm32f30x.h"

int i;
extern uint8_t enabled;

/**
 * User Button
 */
void EXTI0_IRQHandler(void) {
  if ((EXTI_GetITStatus(USER_BUTTON_EXTI_LINE) == SET)&&(STM_EVAL_PBGetState(BUTTON_USER) != RESET)) {
    /* Delay */
    for(i=0; i<0x7FFFF; i++);

    while(STM_EVAL_PBGetState(BUTTON_USER) != RESET);

    for(i=0; i<0x7FFFF; i++);

    if (enabled)
      enabled = 0;
    else
      enabled = 1;

    EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);

  }
}

/**
 * Set LED3 and LED10 on Hard Fault
 */
void HardFault_Handler(void) {
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOn(LED10);
  }
}
