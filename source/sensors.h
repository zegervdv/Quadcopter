/**
 * sensors.h
 *
 * Configure and read data from the onboard sensors
 * of the STM32F3 Discovery
 */

#include "stm32f3_discovery_l3gd20.h"
#include "stm32f3_discovery_lsm303dlhc.h"


union float_to_bytes {
  float f;
  char bytes[sizeof(float)];
};

/**
 * Initialize the onboard L3GD20 gyroscope
 */
void gyroscope_init(void);


/**
 * Read raw data from the gyroscope
 * data - uint8_t array of size 6
 */
void gyroscope_read(uint8_t* data);

/**
 * Convert raw gyroscope data to float with proper scaling
 * data - uint8_t array with raw data bytes
 * value - float array with scaled values
 */
void gyroscope_to_float(uint8_t* data, float* value);

/**
 * Initialize the LSM303DLHC Compass
 */
void compass_init(void);

/**
 * Read raw data from compass
 * data - uint8_t array of size 6
 */
void compass_read(uint8_t* data);

/**
 * Convert raw data from compass to float and scale
 * data - uint8_t array with raw data bytes
 * value - array of 3 floats
 */
void compass_to_float(uint8_t* data, float* value);

/**
 * Initialize the LSM303DLHC Accelerometer
 */
void accelerometer_init(void);

/**
 * Read raw data from accelerometer
 * data - uint8_t array of size 6
 */
void accelerometer_read(uint8_t* data);
