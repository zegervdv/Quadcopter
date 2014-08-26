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

  while(1) {
    /* Loop and wait for interrupts */
    uint8_t stats[BUFFERSIZE];
    uint8_t gyro_data[GYROBUFFER] = {0};
    uint8_t comp_data[COMPBUFFER] = {0};
    uint8_t acc_data[ACCBUFFER] = {0};
    sensor_data data;
    command_typedef command = {0};

    // Read sensors
    gyroscope_read(gyro_data);
    compass_read(comp_data);
    accelerometer_read(acc_data);

    sensors_format_data(gyro_data, comp_data, acc_data, &data);

    //memcpy(stats, &data.roll, BUFFERSIZE);
	float a=1.12;
	memcpy(stats, &a, sizeof(float));
	a=2.23;
	memcpy(stats+sizeof(float), &a, sizeof(float));
	a=3.34;
	memcpy(stats+2*sizeof(float), &a, sizeof(float));
	a=4.45;
	memcpy(stats+3*sizeof(float), &a, sizeof(float));
	a=5.56;
	memcpy(stats+4*sizeof(float), &a, sizeof(float));
	
	if (bluetooth_connected())
      bluetooth_write(stats, BUFFERSIZE);

#ifndef DEBUG
    if (enabled) {
#endif

      if ((bluetooth_connected()) && (bluetooth_check_integrity(command_bytes, CONTROL_MSG_SIZE, command_bytes[CONTROL_MSG_SIZE]))) {
        // Convert received bytes to command
        controls_format(command_bytes, &command);
      }

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

      // Reset command bytes
      memset(command_bytes, 0, CONTROL_MSG_SIZE);
#ifndef DEBUG
    }else {
      // Animation
    }
#endif
    // PID tuning
    Delay(10);

    // Reset Watchdog
    IWDG_ReloadCounter();
  }
}

