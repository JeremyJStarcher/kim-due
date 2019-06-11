#include <stdio.h>
#include <stdlib.h>

#include "MemIoRom.h"

void MemIoRom::install(
    uint16_t start_range,
    uint16_t end_range,
    const uint8_t data[] PROGMEM)
{
    this->start_range = start_range;
    this->end_range = end_range;

    size_t len = end_range - start_range;
    this->data = (uint8_t *)malloc(len);

     printf("Start: %04x  end: %04x size: %zu\n", start_range, end_range, len);

    for (size_t i = 0; i < len; i++)
    {
        this->data[i] = data[i];
        printf("value: %02x \n", data[i]);
    }
}
