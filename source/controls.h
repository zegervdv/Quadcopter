/**
 * controls.h
 * Define functions for controlling and stabilizing the quadcopter
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "stm32f30x_conf.h"

#define CONTROL_MSG_SIZE  (4 * (sizeof(float)) + 2)

/**
 * Indicator modes
 */
#define COMMAND_MODE   (0x00)
#define TAKEOFF_MODE   (0xFF)

/**
 * Command structure
 * roll     - float between -30° and +30° (in radians)
 * pitch    - float between -30° and +30° (in radians)
 * throttle - float between -100 and 100
 * yaw      - float between -180° and +180° (in radians)
 */
typedef struct {
  float roll;
  float pitch;
  float throttle;
  float yaw;
} command_typedef;

extern command_typedef command;
extern uint8_t command_valid;

union unsigned_to_signed {
  uint8_t input[CONTROL_MSG_SIZE - 1];
  float formatted[4];
};


/**
 * Convert raw input data to command_typedef
 * data    - uint8_t array
 * command - command_typedef pointer
 */
void controls_format(uint8_t* data, command_typedef* command);

#endif
