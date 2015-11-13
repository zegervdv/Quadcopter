/**
 * serial.h: Serial communication over serial
 * via RN-42-I/RM
 * 2014, Zeger Van de Vannet
 */

#ifndef serial_H
#define serial_H

#include "stm32f30x.h"
#include "stm32f30x_conf.h"
#include "stm32f3_discovery.h"
#include "stm32f30x_it.h"
#include "controls.h"


extern uint8_t command_bytes[CONTROL_MSG_SIZE + 1];

/**
 * Initialize serial communication
 *
 */
void serial_init(void);


/**
 * Reset the RN42 module
 */
void serial_reset(void);

/**
 * Write data to serial module
 * data - char array with data
 * size - size of the data array
 */
void serial_write(uint8_t* data, int size);

/**
 * Test integrity of the data bytes received using CRC
 * CRC is performed on blocks of 4 bytes, function will pad with zero bytes
 * data_bytes - array of bytes
 * size       - number of bytes
 * checksum   - checksum byte
 *
 * returns 0 if checksums do not match
 */
uint8_t serial_check_integrity(uint8_t* data_bytes, uint8_t size, uint8_t checksum);

/**
 * Test if RN42 module is connected to remote
 *
 * returns 0 if device is not connected
 * returns 1 if device is connected
 */
uint8_t serial_connected(void);

#endif
