/**
 * Quadrocopter STM32 main.c
 * 2014
 */

#include "bluetooth.h"
#include "sensors.h"
#include <string.h>


#define GYROBUFFER    (3 * 2)
#define COMPBUFFER    (3 * 2)
#define ACCBUFFER     (3 * 2)
#define BUFFERSIZE    (GYROBUFFER + COMPBUFFER + ACCBUFFER)


int main(void) {
  union float_to_bytes ftb;

  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED10);

  bluetooth_init();
  gyroscope_init();
  compass_init();
  accelerometer_init();


  while(1) {
    /* Loop and wait for interrupts */
    int i = 100000;
    unsigned char stats[BUFFERSIZE];
    uint8_t gyro_data[GYROBUFFER] = {0};
    uint8_t comp_data[COMPBUFFER] = {0};
    uint8_t acc_data[ACCBUFFER] = {0};
    uint8_t j,k;

    // Read sensors
    gyroscope_read(gyro_data);
    compass_read(comp_data);
    accelerometer_read(acc_data);

    memcpy(stats, gyro_data, GYROBUFFER);
    memcpy(stats + GYROBUFFER, comp_data, COMPBUFFER);
    memcpy(stats + GYROBUFFER + COMPBUFFER, acc_data, ACCBUFFER);

    bluetooth_write(stats, BUFFERSIZE);

    while(i--);
    /* USART_SendData(USART3, 0x74); */

    switch(cmd) {
      case 'z':
        STM_EVAL_LEDToggle(LED3);
        break;
      case 'q':
        STM_EVAL_LEDToggle(LED6);
        break;
      case 'd':
        STM_EVAL_LEDToggle(LED7);
        break;
      case 's':
        STM_EVAL_LEDToggle(LED10);
        break;
      case ' ':
        STM_EVAL_LEDToggle(LED7);
        STM_EVAL_LEDToggle(LED6);
        break;
      default:
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED6);
        STM_EVAL_LEDOff(LED7);
        STM_EVAL_LEDOff(LED10);
    }

    cmd = '\0';
  }
}

