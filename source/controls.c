/**
 * controls.c
 * definition of stabilization and control functions
 */

#include "controls.h"
#include "string.h"

void controls_format(uint8_t* data, command_typedef* command) {
  union unsigned_to_signed uts;
  memcpy(uts.input, &data[1], CONTROL_MSG_SIZE - 1);

  command->roll = (float)uts.formatted[0];
  command->pitch = (float)uts.formatted[1];
  command->throttle = (float)uts.formatted[2];
  command->yaw = (float)uts.formatted[3];
}
