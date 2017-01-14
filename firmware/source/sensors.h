/**
 * sensors.h
 *
 * Configure and read data from the onboard sensors
 * of the STM32F3 Discovery
 */

#include "stm32f30x_conf.h"
#include "stm32f3_discovery_l3gd20.h"
#include "stm32f3_discovery_lsm303dlhc.h"

/* Definition of scaling factors */
#define L3G_Sensitivity_250dps     (float)   114.285f         /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps] */
#define L3G_Sensitivity_500dps     (float)    57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps] */
#define L3G_Sensitivity_2000dps    (float)    14.285f       /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */

#define LSM_Acc_Sensitivity_2g     (float)     1.0f            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     0.5f            /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     0.25f           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     0.0834f         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */

#define LSM303DLHC_M_SENSITIVITY_XY_1_3Ga     1100  /*!< magnetometer X Y axes sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_1_9Ga     855   /*!< magnetometer X Y axes sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_2_5Ga     670   /*!< magnetometer X Y axes sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4Ga       450   /*!< magnetometer X Y axes sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4_7Ga     400   /*!< magnetometer X Y axes sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_5_6Ga     330   /*!< magnetometer X Y axes sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_8_1Ga     230   /*!< magnetometer X Y axes sensitivity for 8.1 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_3Ga      980   /*!< magnetometer Z axis sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_9Ga      760   /*!< magnetometer Z axis sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_2_5Ga      600   /*!< magnetometer Z axis sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4Ga        400   /*!< magnetometer Z axis sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4_7Ga      355   /*!< magnetometer Z axis sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_5_6Ga      295   /*!< magnetometer Z axis sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_8_1Ga      205   /*!< magnetometer Z axis sensitivity for 8.1 Ga full scale [LSB/Ga] */

/**
 * Define scaling for battery voltage
 */
#define BAT_R1    ((float) 560.0)
#define BAT_R2    ((float) 820.0)
#define BAT_SCALE ((float) (BAT_R1 + BAT_R2)/(BAT_R1))

/**
 * Moving average window size
 */
#define AVG_WNDW_SIZE   ((float) 10.0)

typedef struct {
  float roll;
  float pitch;
  float yaw;
} sensor_average;

typedef struct {
  float x_rotation;
  float y_rotation;
  float z_rotation;
  float x_magnetic;
  float y_magnetic;
  float z_magnetic;
  float x_acceleration;
  float y_acceleration;
  float z_acceleration;
  float roll;
  float pitch;
  float yaw;
  float altitude;
  float battery;
} sensor_data;

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
 * data  - uint8_t array with raw data bytes
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

/**
 * Convert raw data from accelerometer to float
 * data  - uint8_t array with raw data bytes
 * value - array of 3 floats
 */
void accelerometer_to_float(uint8_t* data, float* value);

/**
 * Initialze ADC to measure battery voltage
 */
void battery_init(void);

/**
 * Read battery voltage
 * data - float pointer to hold sampled voltage
 */
void battery_read(float* data);

/**
 * Convert raw data to flaot
 * data - sampled battery voltage
 * value - converted battery voltage
 */
void battery_to_float(float* data, float* value);

/**
 * Format raw sensor data into sensor_data struct
 * converts raw data to scaled floats and calculates pitch and roll
 * gyro     - uint8_t array with raw gyroscope data
 * accelero - uint8_t array with raw accelerometer data
 * magneto  - uint8_t array with raw magnetometer data
 * altitude - float with altitude
 * battery  - float with battery voltage
 * data     - pointer to sensor_data struct to hold data
 */
void sensors_format_data(uint8_t* gyro, uint8_t* accelero, uint8_t* magneto, float altitude, float battery, sensor_data* data);
