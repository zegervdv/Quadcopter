#include "MRF89XA.h"
#include "remote.h"
#include "system.h"

void remote_setup(void) {
  /* Start in RX mode by default, 
   * Enable 868MHz band */
  remote_config(RF_GCONREG, RF_CMOD_STDBY | RF_FBS_868MHZ);
  rf_mode = RF_STDBYMODE;
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
   * Size: 64 bytes 
   * 10 bytes threshold (effective min. packet size) */
  remote_config(RF_FIFOCREG, RF_FSIZE_64 | (uint8_t)(2));
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
   * IRQ1 - RX: Threshold hit: go to rx mode
   * IRQ1 - TX: Tx done */
  remote_config(RF_FTXRXIREG, RF_IRQ0RXS_PLREADY | RF_IRQ1RXS_FIFO_THRESHOLD | RF_IRQ1TX_TXDONE);
  /* Fifo configuration:
   * Send when first byte is in fifo, IRQ0 when fifo is not empty */
  remote_config(RF_FTPRIREG, RF_IRQ0TXST_EMPTY | RF_FRPRI_RESERVED | 1);
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
  /* Clear fifo on bad CRC 
   * Enable fifo write in standby mode */
  remote_config(RF_FCRCREG, RF_ACFCRC_ON | RF_FRAWXS_WR);
}

void remote_sync_pll(void) {
  uint8_t data = 0;
  remote_enable_configuration_mode();
  remote_config_read(RF_FTPRIREG, &data);
  /* Set PLL lock bit */
  data |= RF_LSTSPLL;
  remote_config_raw(RF_FTPRIREG, data);
  remote_switch_mode(RF_FRSYNTH);
  /* while ((data & RF_LSTSPLL) != 0) { */
  /*   remote_config_read(RF_FTPRIREG, &data); */
  /* } */
  /* Wait for PLL lock to be set */
  // TODO: Is this correct? set and wait for set?
  data = 0;
  while ((data & RF_LSTSPLL) == 0) {
    remote_config_read(RF_FTPRIREG, &data);
  }
  remote_switch_mode(RF_STDBYMODE);
  remote_disable_configuration_mode();
}
