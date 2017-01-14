/**
 * controls.c
 * definition of stabilization and control functions
 */

#include "controls.h"
#include "string.h"
#include <stdlib.h>
#include "system.h"
#include "pid.h"

/**
 * Unprocessed commands linked list
 */
command_list_t* command_list_start = 0;
command_list_t* command_list_end = 0;


uint8_t command_parse(command_t* command) {
  command_list_t* orig = 0;
  orig = command_list_start;
  if (orig == 0) {
    STM_EVAL_LEDOn(LED4);
    return 0;
  }
  STM_EVAL_LEDOff(LED4);
  switch (orig->raw[0]) {
    case CONTROL_COMMAND_MODE:
      if (command_timeout()) {
        command_reset(command);
      } else {
        command_control(command);
      }
      break;
    case CONTROL_PID_UPDATE:
      command_pid_update(orig);
      break;
    case CONTROL_ECHO:
      command_echo(orig);
    default:
      command_reset(command);
      STM_EVAL_LEDToggle(LED8);
      command_list_start = orig->next;
      free(orig->raw);
      free(orig);
      return 0;
  }
  command_list_start = orig->next;
  free(orig->raw);
  free(orig);
  STM_EVAL_LEDToggle(LED5);
  return 1;
}

void command_reset(command_t* command) {
  memset(command, 0, sizeof(command_t));
}

void command_control(command_t* command) {
  memcpy(&command->roll, &command_list_start->raw[1], sizeof(float));
  memcpy(&command->pitch, &command_list_start->raw[1 + sizeof(float)], sizeof(float));
  memcpy(&command->throttle, &command_list_start->raw[1 + 2 * sizeof(float)], sizeof(float));
  memcpy(&command->yaw, &command_list_start->raw[1 + 3 * sizeof(float)], sizeof(float));
}

void command_pid_update(command_list_t* command) {
  uint8_t i = 0;
  // Disable next PID calculation
  pid_run_flag = 0;
  for (i = 1; i < 4; i++) {
    pid_params[i].iterm = 0;
    pid_params[i].lastInput = 0;
    memcpy(&pid_params[i].kp, &command->raw[2+(i-1)*3], 3);
  }
}

void command_echo(command_list_t* command) {
  uint8_t i = 0;
  uint8_t response[(5*sizeof(float))] = {0};
  for (i = 0; i < 4; i++) {
    response[i] = command->raw[i+1];
  }
  bluetooth_write(response, 5*sizeof(float));
}

uint8_t command_timeout() {
  // TODO: implement
  return 0;
}

