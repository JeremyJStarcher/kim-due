#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#ifdef TARGETWEB
#include "fake-progmen.h"
#include <emscripten/emscripten.h>
#else
#include <avr/pgmspace.h>
#endif

#include "MemIoRom.h"
uint8_t MemIoRom::read(uint16_t address)
{
    unsigned int offset = address - this->start_range;

#ifdef __AVR__
    uint8_t ret = (pgm_read_byte_near(this->data + offset));
#if 0
    Serial.print(" ");
    Serial.print(ret, HEX);
    Serial.print(" ");
    delay(1500);
#endif
    return ret;
#else
    return this->data[offset];
#endif
}

void MemIoRom::install(
    uint16_t start_range,
    uint16_t end_range,
    const uint8_t data[] PROGMEM)
{
    this->start_range = start_range;
    this->end_range = end_range;

    size_t len = end_range - start_range;
    // this->data = (uint8_t *)malloc(len);
    this->data = data;
}
