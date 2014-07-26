/**
 * motors.c
 * Functions to initalize and control motor speeds
 */
#include "motors.h"

void motors_init(void) {
  // Select TIMER1 on pins PA8, PA9, PA10 and PA11
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OC_InitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  // Initialize GPIO pins
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_6);
}

