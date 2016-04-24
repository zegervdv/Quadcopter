#include "remote.h"
#include "stm32f30x_spi.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_exti.h"
#include "stm32f30x_misc.h"

uint8_t rf_mode = 0;

void remote_init(void) {
  GPIO_InitTypeDef gpio_init;
  SPI_InitTypeDef spi_init;
  EXTI_InitTypeDef exti_init;
  NVIC_InitTypeDef nvic_init;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Setup SPI on SPI2 using:
  // - PB13: SCK
  // - PB14: MISO
  // - PB15: MOSI
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5);

  gpio_init.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpio_init);

  SPI_I2S_DeInit(SPI2);
  // TODO: Set correct prescaler
  // TODO: Select polynomial?
  spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi_init.SPI_DataSize = SPI_DataSize_8b;
  spi_init.SPI_CPOL = SPI_CPOL_Low;
  spi_init.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init.SPI_NSS = SPI_NSS_Soft;
  spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init.SPI_CRCPolynomial = 7;
  spi_init.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI2, &spi_init);

  SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
  SPI_Cmd(SPI2, ENABLE);

  // Enable GPIOs for:
  // - PB10: reset
  // - PB11: Data CS
  // - PD9 : Configure CS
  gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpio_init);

  gpio_init.GPIO_Pin = GPIO_Pin_9;
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &gpio_init);

  // Initialize EXTI for:
  // - PD8 : IRQ0
  // - PB12: IRQ1
  gpio_init.GPIO_Pin = GPIO_Pin_8;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Mode = GPIO_Mode_IN;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOD, &gpio_init);

  gpio_init.GPIO_Pin = GPIO_Pin_12;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Mode = GPIO_Mode_IN;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOB, &gpio_init);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource8);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);

  exti_init.EXTI_Line = EXTI_Line12;
  exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
  exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
  exti_init.EXTI_LineCmd = ENABLE;
  EXTI_Init(&exti_init);

  // TODO: Configure correct priorities
  nvic_init.NVIC_IRQChannel = EXTI15_10_IRQn;
  nvic_init.NVIC_IRQChannelPreemptionPriority = 0x0F;
  nvic_init.NVIC_IRQChannelSubPriority = 0x00;
  nvic_init.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_init);

  exti_init.EXTI_Line = EXTI_Line8;
  exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
  exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
  exti_init.EXTI_LineCmd = ENABLE;
  EXTI_Init(&exti_init);

  nvic_init.NVIC_IRQChannel = EXTI9_5_IRQn;
  nvic_init.NVIC_IRQChannelPreemptionPriority = 0x0F;
  nvic_init.NVIC_IRQChannelSubPriority = 0x01;
  nvic_init.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_init);

  // Configure the device
  remote_setup();
  // Lock PLL
  remote_sync_pll();
}

void remote_write(uint8_t* data, int size) {
  remote_switch_mode(RF_TXMODE);
  remote_enable_data_mode();
  remote_send_byte((uint8_t) (size));
  remote_disable_data_mode();
  while(size > 0) {
    remote_enable_data_mode();
    remote_send_byte(*data);
    remote_disable_data_mode();
    data++;
    size--;
  }
  // Hold TX mode
  int j = 0;
  for (j = 0; j < 100; j++);
  remote_switch_mode(RF_RXMODE);
}

void remote_read(uint8_t* data, int size) {
  while(size > 0) {
    remote_enable_data_mode();
    *data = remote_send_byte(SPI_DUMMY_BYTE);
    remote_disable_data_mode();
    data++;
    size--;
  }
}

void remote_config(uint8_t address, uint8_t data) {
  remote_enable_configuration_mode();
  remote_config_raw(address, data);
  remote_disable_configuration_mode();
}

void remote_config_raw(uint8_t address, uint8_t data) {
  remote_send_byte((address << 1) & SPI_WRITE_MASK);
  remote_send_byte(data);
}

void remote_config_read(uint8_t address, uint8_t* data) {
  remote_enable_configuration_mode();
  remote_send_byte((address << 1) | SPI_READ_MASK);
  *data = remote_send_byte(SPI_DUMMY_BYTE);
  remote_disable_configuration_mode();
}

uint8_t remote_send_byte(uint8_t data) {
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  SPI_SendData8(SPI2, data);
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_ReceiveData8(SPI2);
}

void remote_enable_configuration_mode(void) {
  GPIO_WriteBit(GPIOD, GPIO_Pin_9, RESET);
}

void remote_disable_configuration_mode(void) {
  GPIO_WriteBit(GPIOD, GPIO_Pin_9, SET);
}

void remote_enable_data_mode(void) {
  GPIO_WriteBit(GPIOB, GPIO_Pin_11, RESET);
}

void remote_disable_data_mode(void) {
  GPIO_WriteBit(GPIOB, GPIO_Pin_11, SET);
}

void remote_switch_mode(uint8_t mode) {
  if (mode == RF_RXMODE) {
    remote_config(RF_GCONREG, RF_CMOD_RX | RF_FBS_868MHZ);
    rf_mode = RF_RXMODE;
  } else if (mode == RF_TXMODE) {
    remote_config(RF_GCONREG, RF_CMOD_TX | RF_FBS_868MHZ);
    rf_mode = RF_TXMODE;
  } else if (mode == RF_STDBYMODE) {
    remote_config(RF_GCONREG, RF_CMOD_STDBY | RF_FBS_868MHZ);
    rf_mode = RF_STDBYMODE;
  } else if (mode == RF_FRSYNTH) {
    remote_config(RF_GCONREG, RF_CMOD_FREQSYNTH | RF_FBS_868MHZ);
    rf_mode = RF_FRSYNTH;
  }
}
