#include <stdio.h>
#include <stdlib.h>
#include "Riot002.h"
#include "../kim-hardware.h"

#include "../boardhardware.h"
#include "../builtin_display.h"
#include "../serial_display.h"

#define aIoPAD 0x1740
#define aIoPADD 0x1741
#define aIoPBD 0x1742
#define aIoPBDD 0x1743

// #define EMULATE_KEYBOARD

void MemIoRiot002::processIoChange()
{
    uint8_t led;
    uint8_t code;

    led = (ioPBD - 9) >> 1;

    // There is a software demo that fails if we include
    // the ioPADD.
    // trouble is, I'm not sure if the demo is actually working
    // or not, since it was written for a KIM replica, and not a
    // real KIM.
    // However, leaving it out doesn't seem to cause any harm.

    //                 * = 0200
    // 0200   A9 FF      LDA #$FF
    // 0202   8D 40 17   STA $1740
    // 0205   A9 09      LDA #$09
    // 0207   8D 42 17   STA $1742
    // 020A   4C 0A 02   JMP $020A
    // 020D              .END

    code = ioPAD & ioPADD;
    driveLED(led, code);
}

MemIoRiot002::MemIoRiot002()
{
    /* I/O Space of the RIOT002 */
    this->start_range = 0x1740;
    this->end_range = 0x177F;

    this->ioPAD = 0;
    this->ioPADD = 0xFF;
    this->ioPBD = 0;
    this->ioPBDD = 0;
}

uint8_t MemIoRiot002::read(uint16_t address)
{
    if (address == 0x1747)
    {
        // CLKRDI  =$1747,READ TIME OUT BIT,count is always complete...
        return (0xFF);
    }

#ifdef EMULATE_KEYBOARD
    if (address == 0x1740)
    {
        // returns 1 for Keyboard/LED or 0 for Serial terminal
        return (useKeyboardLed);
    }

    serout('%');
    serout('6'); // trap code 6 - read in I/O 002
    return (0);
}
#else
    // The ROM needs the button held down for a certain length of
    // time as debounce technique.  Fake holding the button down.
    const uint8_t HOLDBUTTON_DELAY = 0x08; // Found by trial and error
    static uint8_t ctr = 0;
    static bool holding_key_down = false;
    static uint8_t last_key_value;

    uint8_t key_value;
    uint8_t ret = 0;

    if (holding_key_down)
    {
        key_value = last_key_value;

        ctr--;
        if (ctr == 0)
        {
            holding_key_down = false;
            clearkey();
        }
    }
    else
    {
        key_value = getKIMkey();
        last_key_value = key_value;
        if (key_value != 255)
        {
            holding_key_down = true;
            ctr = HOLDBUTTON_DELAY;
        }
    }

    switch (address)
    {
    case aIoPAD:
        switch (ioPBD & 0x07)
        {
            /* See the keyboard.cpp for details */
        case 1:
            ret = ~(0x40 >> key_value);
            break;
        case 3:
            ret = ~(0x40 >> (key_value - 7));
            break;
        case 5:
            ret = ~(0x40 >> (key_value - 14));
            break;
        case 7:
            ret = useKeyboardLed ? 0xFF : 0xFE;
            break;
        default:
            ret = 0xFF;
            break;
        }

        ret &= ~ioPADD;
        break;

    case aIoPADD:
        ret = ioPADD;
        break;

    case aIoPBD:
        ret = ioPBD;
        ret &= ~ioPBDD;
        break;

    case aIoPBDD:
        ret = ioPBDD;
        break;

    case 0x1706:
        //  ret = ~((UInt8)hwCycles);
        break;

    default:
        ret = 0xff;
        break;
    }

    return ret;
#endif
}

void MemIoRiot002::write(uint16_t address, uint8_t value)
{
    switch (address)
    {
    case aIoPAD:

        /*
            * ioPDB set to         drives digit
            * ------------         ------------
            * xxx1001x               0 (leftmost)
            * xxx1010x               1
            * xxx1011x               2
            * xxx1100x               3
            * xxx1101x               4
            * xxx1110x               5 (rightmost)
            * */

        ioPAD = value;
        processIoChange();

        break;
    case aIoPADD:
        ioPADD = value;
        processIoChange();
        break;

    case aIoPBD:
        ioPBD = value & ioPBDD;
        processIoChange();
        break;

    case aIoPBDD:
        ioPBDD = value;
        processIoChange();
        break;

    default:
        break;
    }
}
