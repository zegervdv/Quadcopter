/**
 * controls.h
 * Define functions for controlling and stabilizing the quadcopter
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "stm32f30x_conf.h"
#include "remote.h"

/**
 * Control message sizes
 */
#define CONTROL_MSG_SIZE  ((uint8_t)(4 * sizeof(float)))

enum type {
  REMOTE_CONTROL = 0,
  TAKE_OFF       = 1,
  PID_CONF       = 4,
  RF_CONF        = 5,
  PASS_THROUGH   = 6
};

/**
 * Control structure: movement controls
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
} control_t;

/**
 * Command structure
 */
typedef struct {
  uint8_t valid : 1;
  enum type type : REMOTE_HEADER_TYPE_SIZE;
  uint8_t length;
  uint8_t data[RF_MAX_PACKET_SIZE];
} command_t;

/**
 * Global command data variable
 */
extern command_t command;

union unsigned_to_signed {
  uint8_t input[CONTROL_MSG_SIZE];
  float formatted[4];
};

/**
 * Parse and process commands from remote module
 * control - control formatting
 * returns 1 if succesfull
 */
uint8_t process_commands(control_t* control);


/**
 * Convert raw input data to command_typedef
 * data    - uint8_t array
 * control - control_t pointer
 */
void controls_format(uint8_t* data, control_t* control);

#endif
