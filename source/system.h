/**
 * system.h: initialize and configure all parts of the quadcopter
 */

#include "bluetooth.h"
#include "controls.h"
#include "motors.h"
#include "sensors.h"
#include "pid.h"
#include "stm32f30x_it.h"
#include "stm32f30x_iwdg.h"

#define WDG_CLK             (uint32_t)(40e3)
#define WDG_PERIOD          (uint32_t)(WDG_CLK / 32)
#define WDG_RLD_TIME(TIME)  (uint16_t)((uint32_t)((TIME) * WDG_PERIOD) / 1000.0)


/**
 * initialize all components
 */
void quadcopter_init(void);
