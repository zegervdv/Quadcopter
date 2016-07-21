#include <stdio.h>
#include <stdint.h>
#include "../source/pid.h"

#define RUNS (2)

int main () {
  int i = 0;
  int j = 0;
  float retval[3] = {0};
  float input[RUNS][3] = {0};

  pid_init();

  for (i = 0; i < RUNS; i++) {
    input[i][0] = 0.00189493200742;
    input[i][1] = 0.00534345395863;
    input[i][2] = 5.01212644577;
  }

  for (i = 0; i < RUNS; i++) {
    for (j = 0; j < 3; j++) {
      pid_compute(1+j, input[i][j], 0, &retval[j]);
    }
    printf("PID: pitch = %0.6f, roll = %0.6f, yaw = %0.6f\n", (retval[0] * 180.0 / PI), retval[1] * 180.0 / PI, retval[2] * 180.0 / PI);
  }
}

