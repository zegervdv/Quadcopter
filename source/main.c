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
	
	for(i=0;i<8;i++){
		if(cmd & (0x01<<i)){STM_EVAL_LEDOn(LED3+i);}
		else{STM_EVAL_LEDOff(LED3+i);}
	}
	
  }
}

