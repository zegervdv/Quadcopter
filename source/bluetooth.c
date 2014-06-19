/**
 * bluetooth.c
 *
 * Bluetooth communication with RN42 over UART
 * Using USART3 on PC10 and PC11
 */

#include "bluetooth.h"

char cmd;

void init_usart3() {
  // Initialize USART3
  // Tx: PIN PC10
  // Rx: PIN PC11
  USART_InitTypeDef usart3_init_struct;
  GPIO_InitTypeDef gpioc_init_struct;
  NVIC_InitTypeDef NVIC_init_struct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  gpioc_init_struct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  gpioc_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  gpioc_init_struct.GPIO_Mode = GPIO_Mode_AF;
  gpioc_init_struct.GPIO_OType = GPIO_OType_PP;
  gpioc_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &gpioc_init_struct);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_7);

  usart3_init_struct.USART_BaudRate = 115200;
  usart3_init_struct.USART_WordLength = USART_WordLength_8b;
  usart3_init_struct.USART_StopBits = USART_StopBits_1;
  usart3_init_struct.USART_Parity = USART_Parity_No;
  usart3_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  usart3_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART3, &usart3_init_struct);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

  NVIC_init_struct.NVIC_IRQChannel = USART3_IRQn;
  NVIC_init_struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_init_struct.NVIC_IRQChannelSubPriority = 0;
	NVIC_init_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_init_struct);

  USART_Cmd(USART3, ENABLE);
}

void init_bluetooth() {
  // Configure USART3
  init_usart3();
}

/**
 * USART3 Interrupt handler
 * TODO: Configure controls here
 */
void USART3_IRQHandler(void) {
  if(USART_GetITStatus(USART3, USART_IT_RXNE)) {
		cmd = USART3->RDR; // the character from the USART1 data register is saved in t
  }
}

