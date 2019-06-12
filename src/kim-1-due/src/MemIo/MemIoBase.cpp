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
    // This version is known NOT to work with negative numbers.
    // I'm OK with that.
    return ((x - this->start_range) <= (this->end_range - this->start_range));

    // Version for negative numbers
    // return ((x-high)*(x-low) <= 0);
}
