/**
 * Communication with remote
 * using spi interface with 2 external interrupt
 */

#ifndef REMOTE_H
#define REMOTE_H

#include "controls.h"

/**
 * SPI start/stop bit
 * SPI read/write bit
 */
#define SPI_START_MASK            (uint8_t)(0x7F)
#define SPI_STOP_MASK             (uint8_t)(0xFE)
#define SPI_WRITE_MASK            (uint8_t)(0xBF)
#define SPI_READ_MASK             (uint8_t)(0x40)

/**
 * SPI Constants
 */
#define DUMMY_BYTE                (uint8_t)(0x00)

/**
 * RF Mode
 */
#define RF_TXMODE                 (uint8_t)(0x01)
#define RF_RXMODE                 (uint8_t)(0x02)

/**
 * RF packet specifications
 */
#define RF_MAX_PACKET_SIZE        (uint8_t)(63)
#define RF_SYNCWRD_3              (uint8_t)(0x55)
#define RF_SYNCWRD_2              (uint8_t)(0x55)
#define RF_SYNCWRD_1              (uint8_t)(0x00)
#define RF_SYNCWRD_0              (uint8_t)(0x00)
#define RF_MAX_PLLEN              (uint8_t)(32)

/**
 * Config register addresses
 */
#define RF_GCONREG                (uint8_t)(0x00)
#define RF_DMODREG                (uint8_t)(0x01)
#define RF_FDEVREG                (uint8_t)(0x02)
#define RF_BRSREG                 (uint8_t)(0x03)
#define RF_FLTHREG                (uint8_t)(0x04)
#define RF_FIFOCREG               (uint8_t)(0x05)
#define RF_R1CREG                 (uint8_t)(0x06)
#define RF_P1CREG                 (uint8_t)(0x07)
#define RF_S1CREG                 (uint8_t)(0x08)
#define RF_R2CREG                 (uint8_t)(0x09)
#define RF_P2CREG                 (uint8_t)(0x0A)
#define RF_S2CREG                 (uint8_t)(0x0B)
#define RF_PACREG                 (uint8_t)(0x0C)
#define RF_FTXRXIREG              (uint8_t)(0x0D)
#define RF_FTPRIREG               (uint8_t)(0x0E)
#define RF_RSTHIREG               (uint8_t)(0x0F)
#define RF_FILCREG                (uint8_t)(0x10)
#define RF_PFCREG                 (uint8_t)(0x11)
#define RF_SYNCREG                (uint8_t)(0x12)
#define RF_RSTSREG                (uint8_t)(0x13)
#define RF_RSVREG                 (uint8_t)(0x14)
#define RF_OOKCREG                (uint8_t)(0x15)
#define RF_SYNCV31REG             (uint8_t)(0x16)
#define RF_SYNCV23REG             (uint8_t)(0x17)
#define RF_SYNCV15REG             (uint8_t)(0x18)
#define RF_SYNCV07REG             (uint8_t)(0x19)
#define RF_TXCONREG               (uint8_t)(0x1A)
#define RF_CLKOREG                (uint8_t)(0x1B)
#define RF_PLOADREG               (uint8_t)(0x1C)
#define RF_NADDSREG               (uint8_t)(0x1D)
#define RF_PKTCREG                (uint8_t)(0x1E)
#define RF_FCRCREG                (uint8_t)(0x1F)

/**
 * Config register values
 */
#define RF_CMOD_STDBY             (uint8_t)(0x20)
#define RF_CMOD_FREQSYNTH         (uint8_t)(0x40)
#define RF_CMOD_RX                (uint8_t)(0x60)
#define RF_CMOD_TX                (uint8_t)(0x80)
#define RF_FBS_868MHZ             (uint8_t)(0x10)
#define RF_RPS_EN_2               (uint8_t)(0x01)
#define RF_RPS_EN_1               (uint8_t)(0x00)
#define RF_MODSEL_OOK             (uint8_t)(0x40)
#define RF_MODSEL_FSK             (uint8_t)(0x80)
#define RF_OOKTYP_FIX             (uint8_t)(0x00)
#define RF_OOKTYP_PEAK            (uint8_t)(0x08)
#define RF_OOKTYP_AVG             (uint8_t)(0x10)
#define RF_DMODE_PCKT             (uint8_t)(0x04)
#define RF_DMODE_BUFF             (uint8_t)(0x20)
#define RF_IFGAIN_0DB             (uint8_t)(0x00)
#define RF_IFGAIN_4DB             (uint8_t)(0x01)
#define RF_IFGAIN_9DB             (uint8_t)(0x02)
#define RF_IFGAIN_13DB            (uint8_t)(0x03)
#define RF_FSIZE_64               (uint8_t)(0xC0)
#define RF_FSIZE_48               (uint8_t)(0x80)
#define RF_FSIZE_32               (uint8_t)(0x40)
#define RF_FSIZE_16               (uint8_t)(0x00)
#define RF_IRQ0RXS_SYNC           (uint8_t)(0xC0)
#define RF_IRQ0RXS_FIFOEMPTY      (uint8_t)(0x80)
#define RF_IRQ0RXS_WRITEBYTE      (uint8_t)(0x40)
#define RF_IRQ0RXS_PLREADY        (uint8_t)(0x00)
#define RF_IRQ1RXS_FIFO_THRESHOLD (uint8_t)(0x30)
#define RF_IRQ1RXS_RSSI           (uint8_t)(0x20)
#define RF_IRQ1RXS_FIFOFULL       (uint8_t)(0x10)
#define RF_IRQ1RXS_CRCOK          (uint8_t)(0x00)
#define RF_IRQ1TX_TXDONE          (uint8_t)(0x08)
#define RF_IRQ1TX_FIFOFULL        (uint8_t)(0x00)
#define RF_IRQ0TXST_EMPTY         (uint8_t)(0x00)
#define RF_FRPRI_RESERVED         (uint8_t)(0x08)
#define RF_PFCREG_RESERVED        (uint8_t)(0x08)
#define RF_POLFILEN_ON            (uint8_t)(0x80)
#define RF_POLFILEN_OFF           (uint8_t)(0x00)
#define RF_PASFILV_378KHZ         (uint8_t)(0xA0)
#define RF_BUTFILV_100KHZ         (uint8_t)(0x03)
#define RF_POLCFV_100KHZ          (uint8_t)(0x30)
#define RF_PFC_RESERVED           (uint8_t)(0x08)
#define RF_SYNCREN_ON             (uint8_t)(0x20)
#define RF_SYNCREN_OFF            (uint8_t)(0x00)
#define RF_SYNCWSZ_32             (uint8_t)(0x18)
#define RF_SYNCWSZ_24             (uint8_t)(0x10)
#define RF_SYNCWSZ_16             (uint8_t)(0x08)
#define RF_SYNCWSZ_8              (uint8_t)(0x00)
#define RF_SYNCTEN_3              (uint8_t)(0x06)
#define RF_SYNCTEN_2              (uint8_t)(0x04)
#define RF_SYNCTEN_1              (uint8_t)(0x02)
#define RF_SYNCTEN_0              (uint8_t)(0x00)
#define RF_TXIPOLFV_125KHZ        (uint8_t)(0x40)
#define RF_TXOPVAL_10DBM          (uint8_t)(0x02)
#define RF_CLKOCNTRL_OFF          (uint8_t)(0x00)
#define RF_MCHSTREN_ON            (uint8_t)(0x80)
#define RF_MCHSTREN_OFF           (uint8_t)(0x00)
#define RF_PKTLENF_VAR            (uint8_t)(0x80)
#define RF_PKTLENF_FIX            (uint8_t)(0x00)
#define RF_PRESIZE_4              (uint8_t)(0x60)
#define RF_WHITEON_ON             (uint8_t)(0x10)
#define RF_WHITEON_OFF            (uint8_t)(0x00)
#define RF_CHKCRCEN_ON            (uint8_t)(0x08)
#define RF_CHKCRCEN_OFF           (uint8_t)(0x00)
#define RF_ADDFIL_OFF             (uint8_t)(0x00)
#define RF_ACFCRC_OFF             (uint8_t)(0x80)
#define RF_ACFCRC_ON              (uint8_t)(0x00)

/**
 * Control register masks
 */
#define RF_FIFOFULL               (uint8_t)(0x04)
#define RF_FIFOEMPTY              (uint8_t)(0x02)
#define RF_FOVRRUN                (uint8_t)(0x01)
#define RF_FIFOFSC                (uint8_t)(0x40)
#define RF_TXDONE                 (uint8_t)(0x20)
#define RF_RIRQS                  (uint8_t)(0x04)
#define RF_LSTSPLL                (uint8_t)(0x02)
#define RF_FRWAXS                 (uint8_t)(0x40)

extern uint8_t rf_mode;

/**
 * Initialize RF communication protocols
 */
void remote_init(void);

/**
 * Send data via RF module
 * data - char array of data bytes to be send
 * size - number of bytes
 */
void remote_write(uint8_t* data, int size);


/**
 * Read data from RF module
 * data - char array for data bytes
 * size - maximum number of bytes to read
 */
void remote_read(uint8_t* data, int size);

/**
 * Write configuration options to RF module
 * address - address byte
 * data    - configuration byte
 */
void remote_config(uint8_t address, uint8_t data);

/**
 * Send a single byte over SPI
 * Use DUMMY_BYTE to read
 * data - data byte
 * returns the read byte
 */
uint8_t remote_send_byte(uint8_t data);

/**
 * Send default configuration
 */
void remote_setup(void);

/**
 * Select configuration mode
 */
void remote_enable_configuration_mode(void);

/**
 * Disable configuration mode
 */
void remote_disable_configuration_mode(void);

/**
 * Select data mode
 */
void remote_enable_data_mode(void);

/**
 * Disable data mode
 */
void remote_disable_data_mode(void);

#endif
