/**
 * system.c: initialize and configure all parts of the quadcopter
 */

#include "system.h"

void TIM_init() {
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_EnableIRQ(TIM3_IRQn);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 12000 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 6000 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_Cmd(TIM3, ENABLE);
}

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

  STM_EVAL_LEDOn(LED6);
  STM_EVAL_LEDOn(LED7);
  // Initialize SysTick
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  // Initialize PID Timer
  TIM_init();

  // Initialize Peripherals
  bluetooth_init();
  gyroscope_init();
  compass_init();
  accelerometer_init();
  battery_init();
  motors_init();

  // Initialize PID tuning
  pid_init();

  // Initialize User Button
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

  // Clear reset flag
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    RCC_ClearFlag();

  motors_arm_escs();

  // Setup Independent Watchdog
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(WDG_RLD_TIME(250));

  IWDG_ReloadCounter();
  IWDG_Enable();
  STM_EVAL_LEDOff(LED6);
  STM_EVAL_LEDOff(LED7);
}

