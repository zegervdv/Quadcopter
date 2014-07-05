/**
 * Quadrocopter STM32 main.c
 * 2014
 */

#include "bluetooth.h"
#include "sensors.h"
#include "controls.h"
#include <string.h>


#define GYROBUFFER    (3 * 2)
#define COMPBUFFER    (3 * 2)
#define ACCBUFFER     (3 * 2)
#define BUFFERSIZE    (GYROBUFFER + COMPBUFFER + ACCBUFFER)


int main(void) {

  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED8);
  STM_EVAL_LEDInit(LED9);
  STM_EVAL_LEDInit(LED10);

  bluetooth_init();
  gyroscope_init();
  compass_init();
  accelerometer_init();


  while(1) {
    /* Loop and wait for interrupts */
    int i = 100000;
    uint8_t stats[BUFFERSIZE];
    uint8_t gyro_data[GYROBUFFER] = {0};
    uint8_t comp_data[COMPBUFFER] = {0};
    uint8_t acc_data[ACCBUFFER] = {0};
    sensor_data data;
    command_typedef command;

    // Read sensors
    gyroscope_read(gyro_data);
    compass_read(comp_data);
    accelerometer_read(acc_data);

    memcpy(stats, gyro_data, GYROBUFFER);
    memcpy(stats + GYROBUFFER, comp_data, COMPBUFFER);
    memcpy(stats + GYROBUFFER + COMPBUFFER, acc_data, ACCBUFFER);

    bluetooth_write(stats, BUFFERSIZE);

    sensors_format_data(gyro_data, comp_data, acc_data, &data);
   
    if (bluetooth_check_integrity(command_bytes, CONTROL_MSG_SIZE, command_bytes[CONTROL_MSG_SIZE])) {
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

    while(i--);
    // Reset command bytes
    memset(command_bytes, 0, CONTROL_MSG_SIZE);

  }
}

