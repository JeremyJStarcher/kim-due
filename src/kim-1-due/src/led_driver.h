#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

void init_display(void);
void clear_display(void);
void driveLED(uint8_t, uint8_t);

#endif