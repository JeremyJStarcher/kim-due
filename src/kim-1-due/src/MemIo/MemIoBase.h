#ifndef MEM_IO_BASE_H
#define MEM_IO_BASE_H

#include <stdint.h>

class MemIoBase
{
    uint16_t start_range;
    uint16_t end_range;

public:
    MemIoBase();
     uint8_t read(uint16_t address) ;
     void write(uint16_t address, uint8_t value);
};

#endif
