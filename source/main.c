/**
 * Quadrocopter STM32 main.c
 * 2014
 */

#include "bluetooth.h"


int main(void) {
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED10);

  bluetooth_init();


  while(1) {
    /* Loop and wait for interrupts */
    int i = 100000;
    
    while(i--); 
    /* USART_SendData(USART3, 0x74); */

    switch(cmd) {
      case 'z':
        STM_EVAL_LEDToggle(LED3);
        break;
      case 'q':
        STM_EVAL_LEDToggle(LED6);
        break;
      case 'd':
        STM_EVAL_LEDToggle(LED7);
        break;
      case 's':
        STM_EVAL_LEDToggle(LED10);
        break;
    }
    
    cmd = '\0';
  }
}

