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
uint8_t getAkey(void); // for serial port get normal ASCII keys

extern uint8_t useKeyboardLed;

extern uint8_t curkey;
extern char threeHex[3][2]; // LED display

#endif
