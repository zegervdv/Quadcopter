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

uint8_t enabled = 0;

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
  control_pid_pwm_typedef pid_pwm_values = {0};

  while(1) {
    /* Loop and wait for interrupts */

    // Read sensors
    gyroscope_read(gyro_data);
    compass_read(comp_data);
    accelerometer_read(acc_data);
    battery_read(&bat_data);

    sensors_format_data(gyro_data, acc_data, comp_data, alt_data, bat_data, &data);

    memcpy(stats, &data.roll, BUFFERSIZE);

    if (bluetooth_connected())
        bluetooth_write(stats, BUFFERSIZE);

#ifndef DEBUG
    if (enabled) {
#endif

      if ((command_valid) && (bluetooth_check_integrity(command_bytes, CONTROL_MSG_SIZE - 1, command_bytes[CONTROL_MSG_SIZE - 1]))) {
        STM_EVAL_LEDOn(LED10);
      }else {
        memset(&command, 0, sizeof(command_typedef));
      }
      // Reset command flag
      command_valid = 0x0;

      // PID tuning
      Delay(10);

      pid_pwm_values.throttle = 2000 + (uint16_t)(command.throttle * 1.5);

      // Set motor speeds
      motors_pid_apply(pid_pwm_values);

      if (command.pitch < 0) {
        STM_EVAL_LEDOn(LED3);
      }else if (command.pitch > 0) {
        STM_EVAL_LEDOn(LED10);
      }else {
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED10);
      }

      if (command.roll > 0) {
        STM_EVAL_LEDOn(LED7);
      }else if (command.roll < 0) {
        STM_EVAL_LEDOn(LED6);
      }else {
        STM_EVAL_LEDOff(LED6);
        STM_EVAL_LEDOff(LED7);
      }

      if (command.yaw > 0) {
        STM_EVAL_LEDOn(LED5);
      }else if (command.yaw < 0) {
        STM_EVAL_LEDOn(LED4);
      }else {
        STM_EVAL_LEDOff(LED4);
        STM_EVAL_LEDOff(LED5);
      }

#ifndef DEBUG
    }else {
      // Animation
    }
#endif

    // Reset Watchdog
    IWDG_ReloadCounter();
  }
}

