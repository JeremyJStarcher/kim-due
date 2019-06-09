#ifndef MEM_IO_ROM_H
#define MEM_IO_ROM_H

#include <stdint.h>
#include "MemIoBase.h"

#ifdef TARGETWEB
#include "fake-progmen.h"
#include <emscripten/emscripten.h>
#else
#include <avr/pgmspace.h>
#endif

class MemIoRom : public MemIoBase
{
    uint16_t start_range;
    uint16_t end_range;

public:
    // MemIoRomType(uint16_t start, uint16_t len, const uint8_t data[] PROGMEM );
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
};

#endif