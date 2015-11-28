#ifndef REMOTE_H
#define REMOTE_H
#include <stdint.h>

#define RF_RXMODE 1
#define RF_TXMODE 0

extern uint8_t rf_mode;

void remote_config(uint8_t address, uint8_t data);

#endif
