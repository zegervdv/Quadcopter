/**
 * controls.h
 * Define functions for controlling and stabilizing the quadcopter
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "stm32f30x_conf.h"

#define CONTROL_MSG_SIZE 4

#define ROLL_SCALE        (float) 0.0040906154343617
#define PITCH_SCALE       (float) 0.0040906154343617
#define THROTTLE_SCALE    1
#define YAW_SCALE         1

/**
 * Command structure
 * roll     - float between - 30° and +29.765625° (in radians)
 * pitch    - float between - 30° and +29.765625° (in radians)
 * throttle - float
 * yaw      - float between - and + (in radians)
 */
typedef struct {
  float roll;
  float pitch;
  float throttle;
  float yaw;
} command_typedef;

union unsigned_to_signed {
  uint8_t input[CONTROL_MSG_SIZE];
  int8_t formatted[CONTROL_MSG_SIZE];
};

/**
 * Convert raw input data to command_typedef
 * data    - uint8_t array
 * command - command_typedef pointer
 */
void controls_format(uint8_t* data, command_typedef* command);

#endif
