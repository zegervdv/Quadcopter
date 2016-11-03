#include "pid.h"
#ifndef SIM
#include "arm_math.h"
#else
#include <stdio.h>
#endif

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
#ifdef SIM
  printf("Pitch KP = %0.6f\n", KP_PITCH);
  printf("Pitch KI = %d\n", KI_PITCH);
  printf("Pitch KD = %d\n", KD_PITCH);
  printf("Roll KP = %0.6f\n", KP_ROLL);
  printf("Roll KI = %d\n", KI_ROLL);
  printf("Roll KD = %d\n", KD_ROLL);
  printf("Yaw KP = %d\n", KP_YAW);
  printf("Yaw KI = %d\n", KI_YAW);
  printf("Yaw KD = %d\n", KD_YAW);
#endif
}

void pid_compute(uint8_t index, float input, float setpoint, float* retVal) {
  pid_params_typedef* pid = &pid_params[index];

  /*Compute all the working error variables*/
  float error = setpoint - input;
  float dInput = (input - pid->lastInput);
  pid->iterm += pid->ki * error;
#ifdef SIM
  printf("Error = %0.6f\n", error);
  printf("dInput = %0.6f\n", dInput);
  printf("pid->iterm = %0.6f\n", pid->iterm);
#endif
  if (pid->iterm > pid->max) pid->iterm = pid->max;
  else if (pid->iterm < pid->min) pid->iterm = pid->min;

  /*Compute PID Output*/
  *retVal = pid->kp * error + pid->iterm - pid->kd * dInput;
#ifdef SIM
  printf("retVal = pid->kp * error + pid->iterm - pid->kd * dInput\n");
  printf("retval = %0.6f\n", *retVal);
#endif
  /* if (*retVal > pid->max) *retVal = pid->max; */
  /* else if (*retVal < pid->min) *retVal = pid->min; */

  /*Remember some variables for next time*/
  pid->lastInput = input;
}
