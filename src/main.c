/***************************************
 * Quadrocopter STM32 main.c
 * 2013-10-11
***************************************/

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_usart.h"
#include "stm32f3_discovery.h"

/* Function prototypes */
void GPIOC_Init(void);
void USART3_Init(void);
void led_toggle(void);

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

/************************************************
 * Initialize GPIO Pin for output
************************************************/
void GPIOC_Init(void)
{
  GPIO_InitTypeDef gpioc_init_struct;
  gpioc_init_struct.GPIO_Pin = GPIO_Pin_7;
  gpioc_init_struct.GPIO_Mode = GPIO_Mode_OUT;
  gpioc_init_struct.GPIO_OType = GPIO_OType_PP;
  gpioc_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
  gpioc_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_Init(GPIOC, &gpioc_init_struct);
  GPIO_SetBits(GPIOC, GPIO_Pin_7);
}

/*************************************************
 * Initialize USART3 with interrupts
 *************************************************/
void USART3_Init(void)
{
  USART_InitTypeDef usart3_init_struct;
  GPIO_InitTypeDef gpioc_init_struct;

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_7);

  /* RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART3 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE); */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Configure PIN10 as Tx */
  gpioc_init_struct.GPIO_Pin = GPIO_Pin_10;
  gpioc_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  gpioc_init_struct.GPIO_Mode = GPIO_Mode_AF;
  gpioc_init_struct.GPIO_OType = GPIO_OType_PP;
  gpioc_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &gpioc_init_struct);

  /* Configure PIN11 as Rx */
  gpioc_init_struct.GPIO_Pin = GPIO_Pin_11;
  gpioc_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  gpioc_init_struct.GPIO_Mode = GPIO_Mode_IN;
  gpioc_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &gpioc_init_struct);

  /* Enable USART3 */
  USART_Cmd(USART3, ENABLE);

  usart3_init_struct.USART_BaudRate = 9600;
  usart3_init_struct.USART_WordLength = USART_WordLength_8b;
  usart3_init_struct.USART_StopBits = USART_StopBits_1;
  usart3_init_struct.USART_Parity = USART_Parity_No;
  usart3_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  usart3_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  USART_Init(USART3, &usart3_init_struct);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

  NVIC_EnableIRQ(USART1_IRQn);
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

/*******************************************
 * USART Interrupt handler
 ******************************************/
void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET )
  {
    if((char)USART_ReceiveData(USART3) == 't')
    {
      led_toggle();
      USART_SendData(USART3, 't');
    }
  }
}
