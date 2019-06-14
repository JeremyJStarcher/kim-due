#include <stdio.h>
#include <stdlib.h>

#include "MemIoRom.h"
uint8_t MemIoRom::read(uint16_t address)
{
    unsigned int offset = address - this->start_range;
    uint8_t val = this->data[offset];

#if 0
    printf("StartRange %04x  ", this->start_range);
    printf("Address: %04x  ", address);
    printf("Offset %04x  ", offset);
    printf("Val %02x  \n", val);
#endif

    return val;
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
