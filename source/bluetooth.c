/**
 * bluetooth.c
 *
 * Bluetooth communication with RN42 over UART
 * Using USART3 on PC10 and PC11
 */

#include "bluetooth.h"
#include "string.h"

#define RST_PIN   GPIO_Pin_12
#define CONN_PIN  GPIO_Pin_0

/**
 * Store received command bytes plus 8 bit CRC
 */
uint8_t command_bytes[CONTROL_MSG_SIZE + 1];

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

/**
 * Initialize PC12 as reset pin for RN42
 * Drive HIGH for normal operation
 */
void init_reset() {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Set Reset pin high
  GPIO_WriteBit(GPIOC, RST_PIN, Bit_SET);
}

/**
 * Initialize PD1 to read RN42 GPIO2
 * Pin is HIGH when bluetooth is connected to remote host
 */
void init_connection() {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = CONN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void bluetooth_init() {
  // Configure USART3
  init_usart3();
  init_reset();
  init_connection();

  // Configure the clock for CRC
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  CRC_DeInit();
  CRC_SetInitRegister(0);
  CRC_PolynomialSizeSelect(CRC_PolSize_8);
  CRC_SetPolynomial(0xD5);
  
  if (!bluetooth_connected())
    bluetooth_reset();
}

void bluetooth_reset() {
  GPIO_WriteBit(GPIOC, RST_PIN, Bit_RESET);
  Delay(50);
  GPIO_WriteBit(GPIOC, RST_PIN, Bit_SET);
}

void bluetooth_write(uint8_t* data, int size) {
  int i = 0;

  // Send synchronization
  for (i = 0; i < 2; i++)
    USART_SendData(USART3, 0x55);


  for (i = 0; i < size; i++) {
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    USART_SendData(USART3, data[i]);
    /* int j = 100000; */
    /* while(j > 0) */
    /*   j--; */
  }
}

uint8_t bluetooth_check_integrity(uint8_t* data_bytes, uint8_t size, uint8_t checksum) {
  uint8_t check = 0;
  uint8_t* end = data_bytes + size;
  CRC_ResetDR();

  while(data_bytes < end) {
    CRC_CalcCRC8bits(*data_bytes++);
  }

  check = (uint8_t)CRC_GetCRC();

  return check == checksum;
}

uint8_t bluetooth_connected(void) {
  return GPIO_ReadInputDataBit(GPIOD, CONN_PIN) == Bit_SET;
}

/**
 * USART3 Interrupt handler
 */
void USART3_IRQHandler(void) {
  if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
    static uint8_t cnt = 0;
		char t = USART_ReceiveData(USART3);

    if ( (t!= '\n') && (cnt < CONTROL_MSG_SIZE + 1) ) {
      command_bytes[cnt] = t;
      cnt++;
    }else {
      cnt = 0;
    }
  }
}


