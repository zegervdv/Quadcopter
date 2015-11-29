/**
 * Communication with remote
 * using spi interface with 2 external interrupt
 */

#ifndef REMOTE_H
#define REMOTE_H

#include "stm32f30x_conf.h"
#include "MRF89XA.h"

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
#define SPI_DUMMY_BYTE            (uint8_t)(0x00)

/**
 * RF Mode
 */
#define RF_TXMODE                 (uint8_t)(0x01)
#define RF_RXMODE                 (uint8_t)(0x02)

/**
 * HEADER SIZE DEFINITIONS
 */
#define REMOTE_HEADER_RSVD_SIZE   (uint8_t)(4)
#define REMOTE_HEADER_TYPE_SIZE   (uint8_t)(3)

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
 * Write configuration options to RF module
 * Must be configured in configuration mode
 * address - address byte
 * data    - configuration byte
 */
void remote_config_raw(uint8_t address, uint8_t data);

/**
 * Send a single byte over SPI
 * Use SPI_DUMMY_BYTE to read
 * data - data byte
 * returns the read byte
 */
uint8_t remote_send_byte(uint8_t data);

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
