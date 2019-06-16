#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void write6502(uint16_t address, uint8_t value);
uint8_t read6502(uint16_t address);

void initKIM(void);
void loadTestProgram(void);

#endif