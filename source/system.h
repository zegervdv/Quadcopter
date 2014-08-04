/**
 * system.h: initialize and configure all parts of the quadcopter
 */

#include "bluetooth.h"
#include "controls.h"
#include "motors.h"
#include "sensors.h"
#include "stm32f30x_it.h"

/**
 * initialize all components
 */
void quadcopter_init(void);
