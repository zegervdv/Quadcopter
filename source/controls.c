/**
 * controls.c
 * definition of stabilization and control functions
 */

#include "controls.h"
#include "string.h"

/**
 * Command data structure
 */
command_t command = {0};

uint8_t process_commands(control_t* control) {
  if (command.valid) {
    switch(command.type) {
      case REMOTE_CONTROL :
        controls_format(command.data, control);
        break;
      case TAKE_OFF :
        // TODO: Set take-off
        break;
      case PID_CONF :
        // TODO: Configure PID values
        break;
      case RF_CONF : 
        // TODO: Configure RF settings
        break;
      case PASS_THROUGH :
        // TODO: Pass data to serial
        break;
      default :
        memset(control, 0, CONTROL_MSG_SIZE);
    }
  }
  return 0;
}

void controls_format(uint8_t* data, control_t* control) {
  union unsigned_to_signed uts;
  memcpy(uts.input, &data, CONTROL_MSG_SIZE);

  control->roll = (float)uts.formatted[0];
  control->pitch = (float)uts.formatted[1];
  control->throttle = (float)uts.formatted[2];
  control->yaw = (float)uts.formatted[3];
}
