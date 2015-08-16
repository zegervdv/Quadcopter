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
#define SPI_START_MASK (uint8_t)(0x7F)
#define SPI_STOP_MASK  (uint8_t)(0xFE)
#define SPI_WRITE_MASK (uint8_t)(0xBF)
#define SPI_READ_MASK  (uint8_t)(0x40)

/**
 * SPI Constants
 */
#define DUMMY_BYTE (uint8_t)(0x00)

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
 * data    - char array of configuration bytes
 * size    - number of bytes
 */
void remote_config(uint8_t address, uint8_t* data, int size);

/**
 * Send a single byte over SPI
 * Use DUMMY_BYTE to read
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
