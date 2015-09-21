/**
 * Quadrocopter STM32 main.c
 * 2014
 */

#include <string.h>
#include "system.h"


#define GYROBUFFER    (3 * 2)
#define COMPBUFFER    (3 * 2)
#define ACCBUFFER     (3 * 2)
#define BUFFERSIZE    (5 * (sizeof(float)))

#ifndef DEBUG
uint8_t enabled = 0;
#else
uint8_t enabled = 1;
#endif


int main(void) {
  // Initialize system
  quadcopter_init();

  uint8_t stats[BUFFERSIZE];
  uint8_t gyro_data[GYROBUFFER] = {0};
  uint8_t comp_data[COMPBUFFER] = {0};
  uint8_t acc_data[ACCBUFFER] = {0};
  float alt_data = 0;
  float bat_data = 0;
  sensor_data data;
  pid_output_typedef pid_output = {0};

  while (1) {
    /* Loop and wait for interrupts */

    // Read sensors
    gyroscope_read(gyro_data);
    compass_read(comp_data);
    accelerometer_read(acc_data);
    battery_read(&bat_data);

    sensors_format_data(gyro_data, acc_data, comp_data, alt_data, bat_data, &data);

    memcpy(stats, &data.roll, BUFFERSIZE);

#ifdef BTEN
    if (bluetooth_connected())
      bluetooth_write(stats, BUFFERSIZE);
#endif
    // TODO: Write stats to RF module

    if (enabled) {

      if ((command_valid)) {
        STM_EVAL_LEDOn(LED10);
      } else {
        memset(&command, 0, sizeof(command_typedef));
      }
      // Reset command flag
      command_valid = 0x0;

      // PID tuning
      if (pid_run_flag) {
        pid_compute(PID_PITCH, data.pitch, command.pitch, &pid_output.pitch);
        pid_compute(PID_ROLL, data.roll, command.roll, &pid_output.roll);
        pid_compute(PID_YAW, data.yaw, command.yaw, &pid_output.yaw);
        pid_run_flag = 0;
        STM_EVAL_LEDOff(LED10);
      }

      // Convert throttle input to value in motor PWM control range
      // The throttle has a range of [-100, 100]
      // The PWM range is [2300, 4000], below 2300 is cut-off
      // Throttle range is [-600, 600] around an offset of 2200 = [1600, 2800]
      pid_output.throttle = 6 * command.throttle + 2200;


      // Set motor speeds
      motors_pid_apply(pid_output);

    } else {
      // Animation
    }

    // Reset Watchdog
    IWDG_ReloadCounter();
  }
}

