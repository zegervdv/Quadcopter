/**
 * controls.c
 * definition of stabilization and control functions
 */

#include "controls.h"
#include "string.h"
#include <stdlib.h>
#include "system.h"

/**
 * Unprocessed commands linked list
 */
command_list_t* command_list_start = 0;
command_list_t* command_list_end = 0;


uint8_t command_parse(command_t* command) {
  STM_EVAL_LEDToggle(LED4);
  command_list_t* orig = 0;
  orig = command_list_start;
  if (orig == 0) {
    return 0;
  }
  switch (orig->raw[0]) {
    case CONTROL_COMMAND_MODE:
      if (command_timeout()) {
        command_reset(command);
      } else {
        command_control(command);
      }
      break;
    case CONTROL_PID_UPDATE:
      command_pid_update();
      break;
    default:
      command_reset(command);
      break;
  }
  command_list_start = orig->next;
  free(orig->raw);
  free(orig);
  return 1;
}

void command_reset(command_t* command) {
  STM_EVAL_LEDOn(LED6);
  memset(command, 0, sizeof(command_t));
}

void command_control(command_t* command) {
  STM_EVAL_LEDOn(LED5);
  memcpy(&command->roll, &command_list_start->raw[1], sizeof(float));
  memcpy(&command->pitch, &command_list_start->raw[1 + sizeof(float)], sizeof(float));
  memcpy(&command->throttle, &command_list_start->raw[1 + 2 * sizeof(float)], sizeof(float));
  memcpy(&command->yaw, &command_list_start->raw[1 + 3 * sizeof(float)], sizeof(float));
}

void command_pid_update() {
  // TODO: implement
}

uint8_t command_timeout() {
  // TODO: implement
  return 0;
}

