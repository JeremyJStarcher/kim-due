#ifndef CPU_INCLUDE_H
#define CPU_INCLUDE_H

#include "Arduino.h"
#define ONBOARD_RAM 1024

uint16_t getpc();
uint8_t getop();
void exec6502(int32_t tickcount);
void reset6502();
void nmi6502();
void initKIM(void);
void loadTestProgram(void);
extern void driveLEDs();
extern void driveCalcLEDs(uint8_t reg);
void scanKeys();
void nmi6502(void);

extern uint8_t RAM[ONBOARD_RAM]; // main 1KB RAM		 0x000-0x3FF
extern uint16_t pc;       // 6502 pc
extern uint8_t SSTmode;
extern uint8_t useKeyboardLed; // 0 to use Serial port or 1 for HEX digits.

#endif