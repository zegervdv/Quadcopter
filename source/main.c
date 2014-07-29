/**
 * Quadrocopter STM32 main.c
 * 2014
 */

#include "stm32f30x_it.h"
#include "bluetooth.h"
#include "sensors.h"
#include "controls.h"
#include "motors.h"
#include <string.h>


#define GYROBUFFER    (3 * 2)
#define COMPBUFFER    (3 * 2)
#define ACCBUFFER     (3 * 2)
#define BUFFERSIZE    (GYROBUFFER + COMPBUFFER + ACCBUFFER)

uint8_t enabled = 0;

int main(void) {
  // Enable LEDS
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED8);
  STM_EVAL_LEDInit(LED9);
  STM_EVAL_LEDInit(LED10);

  // Initialize SysTick
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  // Initialize Peripherals
  bluetooth_init();
  gyroscope_init();
  compass_init();
  accelerometer_init();
  motors_init();

  // Initialize User Button
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
  
  while(1) {
    /* Loop and wait for interrupts */
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

    if (bluetooth_connected() == Bit_SET)
      bluetooth_write(stats, BUFFERSIZE);

    sensors_format_data(gyro_data, comp_data, acc_data, &data);

#ifndef DEBUG
    if (enabled) {
#endif

      if ((bluetooth_connected() == Bit_SET) && (bluetooth_check_integrity(command_bytes, CONTROL_MSG_SIZE, command_bytes[CONTROL_MSG_SIZE]))) {
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
    Delay(1000);
  }
}

