/**
 * motors.c
 * Functions to initalize and control motor speeds
 */
#include "motors.h"

void motors_init(void) {
  // Select TIM8 on pins PC6, PC7, PC8 and PC9
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  // Initialize GPIO pins
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Select the Alternate Function TIM8 for pins
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_4);

  // Set TIM8 scaler
  TIM_TimeBaseStructure.TIM_Prescaler = PWM_PRESCALER;
  TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

  // Initialize Output Comparators
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;

  // Initalize channels
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);

  TIM_OC2Init(TIM8, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

  TIM_OC3Init(TIM8, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

  TIM_OC4Init(TIM8, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM8, DISABLE);
  TIM_CtrlPWMOutputs(TIM8, ENABLE);
  TIM_Cmd(TIM8, ENABLE);
}

void motors_set_speed(uint8_t motor, uint32_t speed) {
  if(IS_VALID_SPEED(speed)) {
    if(MOTOR_LEFT_FRONT & motor)
      TIM_SetCompare2(TIM8, speed);
    if(MOTOR_RIGHT_FRONT & motor)
      TIM_SetCompare3(TIM8, speed);
    if(MOTOR_LEFT_BACK & motor)
      TIM_SetCompare1(TIM8, speed);
    if(MOTOR_RIGHT_BACK & motor)
      TIM_SetCompare4(TIM8, speed);
  }
}
