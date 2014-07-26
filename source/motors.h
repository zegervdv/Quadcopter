/**
 * motors.h
 * Functions to initalize and control motor speeds
 */

#include "stm32f30x_tim.h"

/**
 * System clock runs at 72MHz
 * Set TIMER frequency at 2MHz
 * Prescaler = SYS_CLK / TIM_FREQ = 36
 * PWM frequency at 200 Hz
 * PWM period = TIM_FREQ / PWM_FREQ = 10000
 * PWM pulse length = 2000 ms
 */
#define SYS_CLK       (uint32_t)(72e6)
#define TIM_FREQ      (uint32_t)(2e6)
#define PWM_PRESCALER (uint32_t)(SYS_CLK / TIM_FREQ)
#define PWM_FREQ      (uint32_t)(200)
#define PWM_PERIOD    (uint32_t)(TIM_FREQ / PWM_FREQ)
#define PWM_PULSE     (uint32_t)(PWM_PERIOD / (1000.0/PWM_FREQ))

/**
 * Identifiers for motors
 */
#define MOTOR_LEFT_FRONT    (uint8_t)(0x1)
#define MOTOR_RIGHT_FRONT   (uint8_t)(0x2)
#define MOTOR_LEFT_BACK     (uint8_t)(0x4) 
#define MOTOR_RIGHT_BACK    (uint8_t)(0x8)


/**
 * Initialize timers for PWM control
 */
void motors_init(void);


/**
 * Set PWM duty cycle
 * TODO: Use TIM_SetCompare<CHANNEL>();
 * motor - Select one or more motors to set:
 *         MOTOR_LEFT_FRONT
 *         MOTOR_RIGHT_FRONT
 *         MOTOR_LEFT_BACK
 *         MOTOR_RIGHT_BACK
 * speed - value of speed to be set
 */
void motors_set_speed(uint8_t motor, uint16_t speed);
