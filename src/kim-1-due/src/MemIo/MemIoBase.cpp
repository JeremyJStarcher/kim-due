#include "MemIoBase.h"

MemIoBase::MemIoBase()
{
}

void MemIoBase::write(uint16_t address, uint8_t value)
{
}

uint8_t MemIoBase::read(uint16_t address)
{
    return 0xEE;
}

bool MemIoBase::inRange(uint16_t x)
{
    return (x >= this->start_range && x <= this->end_range);
}
