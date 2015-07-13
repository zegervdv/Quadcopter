#include "stm32f30x_conf.h"

#ifndef PID_H
#define PID_H

// PID Sampling time interval (milliseconds)
#define PID_SAMPLE_TIME 1000

// PID Tuning Parameters
#define PID_PITCH (1)
#define KP_PITCH (45000.0 / PI)
#define KI_PITCH 0
#define KD_PITCH 0
#define MIN_PITCH (-500)
#define MAX_PITCH (500)

#define PID_ROLL (2)
#define KP_ROLL (45000.0 / PI)
#define KI_ROLL 0
#define KD_ROLL 0
#define MIN_ROLL (-500)
#define MAX_ROLL (500)

#define PID_YAW (3)
#define KP_YAW 0
#define KI_YAW 0
#define KD_YAW 0
#define MIN_YAW (-PI)
#define MAX_YAW (PI)


/**
 * Store output values
 */
typedef struct {
  uint16_t throttle;
  uint16_t pitch;
  uint16_t roll;
  uint16_t yaw;
} pid_output_typedef;

/**
 * Initialize the PID tuning parameters
 */
void pid_init(void);

/**
 * Compute the PID output for given index
 * index - parameter index value
 * input - sensor data value
 * setpoint - new value
 * retVal - result from PID loop
 */
void pid_compute(uint8_t index, float input, float setpoint, uint16_t* retVal);

#endif
