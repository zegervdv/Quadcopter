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
  GPIO_Init(GPIOD, &gpio_init);

  gpio_init.GPIO_Pin = GPIO_Pin_12;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_init.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &gpio_init);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource8);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);

  exti_init.EXTI_Line = EXTI_Line8 | EXTI_Line12;
  exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
  exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
  exti_init.EXTI_LineCmd = ENABLE;
  EXTI_Init(&exti_init);

  nvic_init.NVIC_IRQChannel = EXTI9_5_IRQn;
  nvic_init.NVIC_IRQChannelPreemptionPriority = 0x0F;
  nvic_init.NVIC_IRQChannelSubPriority = 0x0F;
  nvic_init.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_init);

  nvic_init.NVIC_IRQChannel = EXTI15_10_IRQn;
  nvic_init.NVIC_IRQChannelPreemptionPriority = 0x0F;
  nvic_init.NVIC_IRQChannelSubPriority = 0x0F;
  nvic_init.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_init);
}

void remote_write(uint8_t* data, int size) {
  while(size > 0) {
    remote_enable_data_mode();
    remote_send_byte(*data);
    remote_disable_data_mode();
    data++;
    size--;
  }
}

void remote_read(uint8_t* data, int size) {
  while(size > 0) {
    remote_enable_data_mode();
    *data = remote_send_byte(DUMMY_BYTE);
    remote_disable_data_mode();
    data++;
    size--;
  }
}

void remote_config(uint8_t address, uint8_t data) {
  remote_enable_configuration_mode();
  remote_send_byte(address & SPI_START_MASK & SPI_STOP_MASK & SPI_READ_MASK);
  remote_send_byte(data);
  remote_disable_configuration_mode();
}

uint8_t remote_send_byte(uint8_t data) {
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  SPI_SendData8(SPI2, data);
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_ReceiveData8(SPI2);
}

void remote_setup(void) {
  /* Start in RX mode by default, 
   * Enable 868MHz band */
  remote_config(RF_GCONREG, RF_CMOD_RX | RF_FBS_868MHZ);
  rf_mode = RF_RXMODE;
  /* Use FSK modulation
   * Enable packet mode
   * Set gain to 0 dB */
  remote_config(RF_DMODREG, RF_MODSEL_FSK | RF_DMODE_PCKT | RF_IFGAIN_0DB);
  /* Frequency deviation 
   * Fdev = Fxtal / (32 * (1 + FDVAL))
   * => 40 kHz
   * Max allowed 40 kHz */
  remote_config(RF_FDEVREG, 0x09);
  /* Bit rate
   * BR = Fxtal / (64 * (1 + BRVAL))
   * => 25 kbps 
   * Max. allowed 40 kbps in FSK mode */
  remote_config(RF_BRSREG, 0x07);
  /* Floor threshold OOK: n/a */
  /* Fifo configuration
   * Size: 64 bytes */
  remote_config(RF_FIFOCREG, RF_FSIZE_64);
  /* PLL configuration:
   * Frf = 9/8 * Fxtal / (R + 1) * (75 * (P + 1) + S)
   * with Frf = 868MHz & Fxtal = 12.8MHz
   * R = 99
   * P = 79
   * S = 28 */
  remote_config(RF_R1CREG, (uint8_t)(99));
  remote_config(RF_P1CREG, (uint8_t)(79));
  remote_config(RF_S1CREG, (uint8_t)(28));
  /* Interrupt configuration
   * IRQ0 - RX: Payload Ready, i.e. packet has arrived
   * IRQ1 - RX: Fifo overload
   * IRQ1 - TX: Tx done */
  remote_config(RF_FTXRXIREG, RF_IRQ0RXS_PLREADY | RF_IRQ1RXS_FIFOFULL | RF_IRQ1TX_TXDONE);
  /* Fifo configuration:
   * Send when first byte is in fifo, IRQ0 when fifo is not empty */
  remote_config(RF_FTPRIREG, RF_IRQ0TXST_EMPTY | RF_FRPRI_RESERVED);
  /* TODO: Filter configuration */
  /* Single sideband bandwidth of low pass filter
   * Set to 378 kHz
   * Butterworth filter: 100 kHz */
  remote_config(RF_FILCREG, RF_PASFILV_378KHZ | RF_BUTFILV_100KHZ);
  /* Polyphase filter
   * 100 kHz */
  remote_config(RF_PFCREG, RF_POLCFV_100KHZ | RF_PFC_RESERVED);
  /* Sync control
   * Enable sync word recognition
   * Set sync word size to 16 bits
   * Set sync word error tolerance to 0 bits */
  remote_config(RF_SYNCREG, RF_SYNCREN_ON | RF_SYNCWSZ_16 | RF_SYNCTEN_0);
  /* Sync word
   * 0x5555 */
  remote_config(RF_SYNCV31REG, RF_SYNCWRD_3);
  remote_config(RF_SYNCV23REG, RF_SYNCWRD_2);
  remote_config(RF_SYNCV15REG, RF_SYNCWRD_1);
  remote_config(RF_SYNCV07REG, RF_SYNCWRD_0);
  /* TX interpolation filter settings
   * Cut off 125 kHz
   * Fc = 200 * TXIPOLFV/8
   * => 5
   * TX output power: 10 dBm */
  remote_config(RF_TXCONREG, RF_TXIPOLFV_125KHZ | RF_TXOPVAL_10DBM);
  /* Disable clock output */
  remote_config(RF_CLKOREG, RF_CLKOCNTRL_OFF);
  /* Disable Manchester encoding
   * Max. payload length in RX mode: 20 bytes */
  remote_config(RF_PLOADREG, RF_MCHSTREN_OFF | RF_MAX_PLLEN);
  /* Set variable packet length
   * Set preamble to 4 bytes
   * Enable whitening
   * Enable CRC */
  remote_config(RF_PKTCREG, RF_PKTLENF_VAR | RF_PRESIZE_4 | RF_WHITEON_ON | RF_CHKCRCEN_ON | RF_ADDFIL_OFF);
  /* Clear fifo on bad CRC */
  remote_config(RF_FCRCREG, RF_ACFCRC_ON);
}

void remote_switch_mode(uint8_t mode) {
  if (mode == RF_RXMODE) {
    remote_config(RF_GCONREG, RF_CMOD_RX | RF_FBS_868MHZ);
    rf_mode = RF_RXMODE;
  } else if (mode == RF_TXMODE) {
    remote_config(RF_GCONREG, RF_CMOD_TX | RF_FBS_868MHZ);
    rf_mode = RF_TXMODE;
  }
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
