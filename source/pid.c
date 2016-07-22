#include "pid.h"
#include "arm_math.h"

pid_params_typedef pid_params[4];

void pid_init(void) {
  uint8_t i = 0;
  float SampleTimeInSec = ((float)PID_SAMPLE_TIME) / 1000.0;
  for (i = 0; i < 4; i++) {
    pid_params[i].iterm = 0;
    pid_params[i].lastInput = 0;
  }
  // Set PID Parameters
  pid_params[PID_PITCH].kp = KP_PITCH;
  pid_params[PID_PITCH].ki = KI_PITCH * SampleTimeInSec;
  pid_params[PID_PITCH].kd = KD_PITCH / SampleTimeInSec;
  pid_params[PID_PITCH].min = MIN_PITCH;
  pid_params[PID_PITCH].max = MAX_PITCH;
  pid_params[PID_ROLL].kp = KP_ROLL;
  pid_params[PID_ROLL].ki = KI_ROLL * SampleTimeInSec;
  pid_params[PID_ROLL].kd = KD_ROLL / SampleTimeInSec;
  pid_params[PID_ROLL].min = MIN_ROLL;
  pid_params[PID_ROLL].max = MAX_ROLL;
  pid_params[PID_YAW].kp = KP_YAW;
  pid_params[PID_YAW].ki = KI_YAW * SampleTimeInSec;
  pid_params[PID_YAW].kd = KD_YAW / SampleTimeInSec;
  pid_params[PID_YAW].min = MIN_YAW;
  pid_params[PID_YAW].max = MAX_YAW;
}

void pid_compute(uint8_t index, float input, float setpoint, int16_t* retVal) {
  pid_params_typedef* pid = &pid_params[index];

  /*Compute all the working error variables*/
  float error = setpoint - input;
  float dInput = (input - pid->lastInput);
  pid->iterm += pid->ki * error;
  if (pid->iterm > pid->max) pid->iterm = pid->max;
  else if (pid->iterm < pid->min) pid->iterm = pid->min;

  /*Compute PID Output*/
  *retVal = (int16_t)(pid->kp * error + pid->iterm - pid->kd * dInput);
  if (*retVal > pid->max) *retVal = pid->max;
  else if (*retVal < pid->min) *retVal = pid->min;

  /*Remember some variables for next time*/
  pid->lastInput = input;
}
