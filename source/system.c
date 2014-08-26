/**
 * system.c: initialize and configure all parts of the quadcopter
 */

#include "system.h"

void quadcopter_init(void) {
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
  battery_init();
  motors_init();

  // Initialize User Button
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

  // Clear reset flag
  if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    RCC_ClearFlag();

  // Setup Independent Watchdog
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(WDG_RLD_TIME(250));

  IWDG_ReloadCounter();
  IWDG_Enable();
}
