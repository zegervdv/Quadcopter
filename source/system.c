/**
 * system.c: initialize and configure all parts of the quadcopter
 */

#include "system.h"

void TIM_init() {
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Compute the prescaler value */
  uint16_t PrescalerValue = (uint16_t)((SystemCoreClock) / 72000000) - 1;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 40961;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
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

