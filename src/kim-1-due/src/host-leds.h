#ifndef BUILTIN_DISPLAY_H
#define BUILTIN_DISPLAY_H

#include <stdint.h>

void init_display(void);
void clear_display(void);
void driveLED(uint8_t, uint8_t);

#endif