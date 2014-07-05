/**
 * controls.c
 * definition of stabilization and control functions
 */

#include "controls.h"
#include "string.h"

void controls_format(uint8_t* data, command_typedef* command) {
  union unsigned_to_signed uts;
  memcpy(uts.input, data, CONTROL_MSG_SIZE);

  command->roll = (float)uts.formatted[0] / ROLL_SCALE;
  command->pitch = (float)uts.formatted[1] / PITCH_SCALE;
  command->throttle = (float)uts.formatted[2] / THROTTLE_SCALE;
  command->yaw = (float)uts.formatted[3] / YAW_SCALE;
}
