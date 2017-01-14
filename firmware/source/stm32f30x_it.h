/**
 * stm32f30x_it.h
 * Interrupt handlers and timing functions
 */

#ifndef STM32F30X_IT_H
#define STM32F30X_IT_H

#include "stm32f3_discovery.h"
#include "stm32f30x_conf.h"
#include "stm32f30x.h"
#include "bluetooth.h"

extern uint8_t pid_run_flag;

/**
 * Wait for amount of milliseconds
 * nTime - time to wait in milliseconds
 */
void Delay(__IO uint32_t nTime);

void TimingDelay_Decrement(void);

#endif
