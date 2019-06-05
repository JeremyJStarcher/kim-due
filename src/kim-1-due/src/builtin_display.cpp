#include <stdint.h>

#include "builtin_display.h"
#include "cpu.h"
#include "kim-hardware.h"
#include "boardhardware.h"

#if BOARD_LED_MAX7219
#include <SPI.h>
#include "./MAX7219/src/bitBangedSPI.h"
#include "./MAX7219/src/MAX7219.h"
#endif

#ifdef TARGETWEB
#include <emscripten/emscripten.h>
#endif

void convert_led_pattern(void);

const int delaytime = 150;
static uint8_t xlate_led_pattern[256];

uint8_t dig[19] = {
    // bits     6543210
    // digits   abcdefg
    0b01111110, //0
    0b00110000, //1
    0b01101101, //2
    0b01111001, //3
    0b00110011, //4
    0b01011011, //5
    0b01011111, //6
    0b01110000, //7
    0b01111111, //8
    0b01111011, //9
    0b01110111, //a
    0b00011111, //b
    0b01001110, //c
    0b00111101, //d
    0b01001111, //e
    0b01000111, //f
    0b00000001, //g printed as -
    0b00001000, //h printed as _
    0b00000000  //i printed as <space>
};

#ifdef TARGETWEB
void init_display()
{
    convert_led_pattern();

    uint8_t logo[]{
        0b01110111, // A
        0b00000101, // r
        0b00111101, // d
        0b00011100, // u
        0b00010000, // i
        0b00010101, // n
        0b00011101, // o
        0b00000000  // <blank>
    };

    for (int i = 0; i < sizeof(logo) / sizeof(logo[0]); i++)
    {
        // var k = [i, logo[i]].join(", ");
        EM_ASM({
            setLed($0, $1);
        },
               i, logo[i]);
    }

    //  clear_display();
}

void driveLED(uint8_t led, uint8_t n)
{
    // Cheap anti-flicker
    if (n == 0)
    {
        return;
    }

    // Pick off just the bits we need
    led &= 0b00000111;

    if (led > 5)
    {
        return;
    }

    uint8_t p2 = xlate_led_pattern[n];

    EM_ASM({
        setLed($0, $1);
    },
           led, p2);
}

void driveLEDs()
{
    int ledNo;
    int byt, i;
    int out;

    for (byt = 0; byt < 3; byt++)
    {
        for (i = 0; i < 2; i++)
        {
            ledNo = byt * 2 + i;
            char bcd = threeHex[byt][i];
            out = dig[(int)bcd];
            EM_ASM({
                setLed($0, $1);
            },
                   ledNo, out);
        }
    }
}

#endif

#if BOARD_LED_MAX7219

MAX7219 display(1, LED_CS);
static uint8_t t_8_6[] = {0, 1, 2, 3, 5, 6};

void init_display()
{
    display.begin();
    display.setIntensity(6);

    convert_led_pattern();

    uint8_t logo[]{
        0b01110111, // A
        0b00000101, // r
        0b00111101, // d
        0b00011100, // u
        0b00010000, // i
        0b00010101, // n
        0b00011101, // o
        0b00000000  // <blank>
    };

    for (int i = 0; i < sizeof(logo) / sizeof(logo[0]); i++)
    {
        display.sendRawByte(i, logo[i]);
        delay(delaytime);
    }

    clear_display();
}

void driveLEDs()
{
    int ledNo;
    int byt, i;
    int out;

    for (byt = 0; byt < 3; byt++)
    {
        for (i = 0; i < 2; i++)
        {
            ledNo = byt * 2 + i;
            char bcd = threeHex[byt][i];
            out = dig[(int)bcd];
            display.sendRawByte(0, t_8_6[ledNo], out);
        }
    }
}

void driveLED(uint8_t led, uint8_t n)
{
    // Cheap anti-flicker
    if (n == 0)
    {
        return;
    }

    // Pick off just the bits we need
    led &= 0b00000111;

    if (led >= (sizeof(t_8_6) / sizeof(t_8_6[0])))
    {
        return;
    }

    size_t real_led = t_8_6[led];
    display.sendRawByte(real_led, xlate_led_pattern[n]);
}

void clear_display()
{
    display.sendString("");
}

#endif

#if BOARD_WIRED_LED
void init_display()
{
    convert_led_pattern();
}

void driveLEDs()
{
    int led, col, ledNo, currentBit, bitOn;
    int byt, i;

    // 1. initialse for driving the 6 (now 8) 7segment LEDs
    // ledSelect pins drive common anode for [all segments] in [one of 6 LEDs]
    for (led = 0; led < 7; led++)
    {
        pinMode(ledSelect[led], OUTPUT);   // set led pins to output
        digitalWrite(ledSelect[led], LOW); // LOW = not lit
    }
    // 2. switch column pins to output mode
    // column pins are the cathode for the LED segments
    // lame code to cycle through the 3 bytes of 2 digits each = 6 leds
    for (byt = 0; byt < 3; byt++)
        for (i = 0; i < 2; i++)
        {
            ledNo = byt * 2 + i;
            for (col = 0; col < 8; col++)
            {
                pinMode(aCols[col], OUTPUT); // set pin to output
                //currentBit = (1<<(6-col));             // isolate the current bit in loop
                currentBit = (1 << (7 - col)); // isolate the current bit in loop
                bitOn = (currentBit & dig[(int)threeHex[byt][i]]) == 0;
                digitalWrite(aCols[col], bitOn); // set the bit
            }
            digitalWrite(ledSelect[ledNo], HIGH); // Light this LED
            delay(2);
            digitalWrite(ledSelect[ledNo], LOW); // unLight this LED
        }
} // end of function

void clear_display()
{
}
#endif

void convert_led_pattern(void)
{
    // --- LED VERSION ---
    //      .::::-  (6)
    // (1)  :    :  (5)
    //      .::::.  (0)
    // (2)  :    :  (4)
    //      .::::-  (3)

    // --- KIM VERSION ---
    //      .::::-  (0)
    // (5)  :    :  (1)
    //      .::::.  (6)
    // (4)  :    :  (2)
    //      .::::-  (3)

    // As you can see, the LED patters are almost just mirror reverse of
    // each other.
    // Set up a translation table, -- its the fastest way to do it.

    for (int i = 0; i < 256; i++)
    {
        uint8_t n = i;
        int shift;
        uint8_t result = 0;

        for (shift = 0; shift < 8; shift++)
        {
            result <<= 1;
            result |= n & 1;
            n >>= 1;
        }

        result >>= 1;
        xlate_led_pattern[i] = result;
    }
}