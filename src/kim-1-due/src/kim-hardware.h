#ifndef KIM_HARDWARE_H
#define KIM_HARDWARE_H

#ifndef TARGETWEB
#include <Arduino.h>
#endif

#include <stdint.h>

void setupUno(void);
int freeRam(void);
void interpretkeys(void);
uint8_t xkeyPressed(void);
void scanKeys(void);
void clearkey(void);
uint8_t getKIMkey();           // for emulation of KIM keypad
extern uint8_t useKeyboardLed;

extern uint8_t curkey;
extern char threeHex[3][2]; // LED display

#endif
