/***************************************
 * Quadrocopter STM32 main.c
 * 2013-10-11
***************************************/

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_usart.h"
#include "stm32f3_discovery.h"
#include "stm32f30x_misc.h"

/*************************************************
 * Initialize USART3 with interrupts
 *************************************************/
void USART3_Init(void)
{
  USART_InitTypeDef usart3_init_struct;
  GPIO_InitTypeDef gpioc_init_struct;
  NVIC_InitTypeDef NVIC_init_struct;

  /* RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART3 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE); */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Configure PIN10 as Tx, PIN11 as Rx */
  gpioc_init_struct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  gpioc_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  gpioc_init_struct.GPIO_Mode = GPIO_Mode_AF;
  gpioc_init_struct.GPIO_OType = GPIO_OType_PP;
  gpioc_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &gpioc_init_struct);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_7);


  usart3_init_struct.USART_BaudRate = 9600;
  usart3_init_struct.USART_WordLength = USART_WordLength_8b;
  usart3_init_struct.USART_StopBits = USART_StopBits_1;
  usart3_init_struct.USART_Parity = USART_Parity_No;
  usart3_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  usart3_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART3, &usart3_init_struct);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  
  NVIC_init_struct.NVIC_IRQChannel = USART3_IRQn;
  NVIC_init_struct.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
	NVIC_init_struct.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_init_struct.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_init_struct);

  /* Enable USART3 */
  USART_Cmd(USART3, ENABLE);
}


/*********************************************
 * Sample - LED toggle
 ********************************************/
void led_toggle(void)
{
  STM_EVAL_LEDToggle(LED3);
}


int main(void)
{
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDToggle(LED4);

  /* Initialize USART3 */
  USART3_Init();
  
  /* Start by sending first character */
  while(1)
  {
    /* Loop and wait for interrupts */
    int i = 10000000;
    while(i--); 
    USART_SendData(USART3, 0x74);
  }
}

/*******************************************
 * USART Interrupt handler
 ******************************************/
void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE))
  {
		char t = USART3->RDR; // the character from the USART1 data register is saved in t
    
    // Toggle LED3 on reception of data
    led_toggle();

		if(t == 'a'){ 
      // If received data is "a", toggle LED4
      STM_EVAL_LEDToggle(LED4);
		}
		else{ 
      
		}
  }
}
