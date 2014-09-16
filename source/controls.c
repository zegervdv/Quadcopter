/**
 * controls.c
 * definition of stabilization and control functions
 */

#include "controls.h"
#include "string.h"

/**
 * Formatted commands
 */
command_typedef command = {0};

void controls_format(uint8_t* data, command_typedef* command) {
  union unsigned_to_signed uts;
  switch (data[0]) {
    case COMMAND_MODE:
      memcpy(uts.input, &data[1], CONTROL_MSG_SIZE - 2);

      command->roll = (float)uts.formatted[0];
      command->pitch = (float)uts.formatted[1];
      command->throttle = (float)uts.formatted[2];
      command->yaw = (float)uts.formatted[3];
      break;
    case TAKEOFF_MODE:
      // TODO: Set takeoff command
      break;
    default:
      // Unknown mode, reset command
      memset(command, 0, CONTROL_MSG_SIZE);
  }
}
