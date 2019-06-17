#include <stdint.h>

#include "host-leds.h"
#include "cpu.h"
#include "kim-hardware.h"
#include "host-hardware.h"

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

#ifdef TARGETWEB
#include "../../../browser/src/c/host-leds.cpp"
#endif

#if BOARD_LED_MAX7219

MAX7219 display(1, LED_CS);
static uint8_t t_8_6[] = {0, 1, 2, 3, 5, 6};

void init_display()
{
    display.begin();
    display.setIntensity(6);

    convert_led_pattern();

    uint8_t logo[8]{
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

void driveLED(uint8_t led, uint8_t n)
{
    // Cheap anti-flicker
    if (n == 0)
    {
        // return;
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

void driveLED(uint8_t led, uint8_t n)
{
}

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
