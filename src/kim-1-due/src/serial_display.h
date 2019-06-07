#ifndef SERIAL_DISPLAY_H
#define SERIAL_DISPLAY_H

#include <stdint.h>

extern void serout(uint8_t value);
extern void serouthex(uint8_t val);
void serial_scands(void);
void serial_statusline(void);

#endif