#ifndef KIM_HARDWARE_H
#define KIM_HARDARE_H

#include <Arduino.h>

void setupUno(void);
int freeRam(void);
void interpretkeys(void);
uint8_t xkeyPressed(void);

extern uint8_t curkey;
extern char threeHex[3][2]; // LED display

#endif

