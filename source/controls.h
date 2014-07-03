/**
 * controls.h
 * Define functions for controlling and stabilizing the quadcopter
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#define CONTROL_MSG_SIZE 3

/**
 * Command structure
 * longitudinal - signed byte: positive for forward, value indicates speed
 * lateral - signed byte: positive for right
 * normal - signed nibble: positive for up
 * rotation - signed nibble: positive for clockwise (from above)
 */
typedef struct {
  signed char longitudinal;
  signed char lateral;
  signed char normal : 4;
  signed char rotation : 4;
} command_typedef;

union read_command {
  uint8_t input[CONTROL_MSG_SIZE];
  command_typedef formatted;
};

#endif
