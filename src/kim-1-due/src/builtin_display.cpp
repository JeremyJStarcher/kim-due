#include <LedControl.h>

#include "builtin_display.h"
#include "cpu.h"
#include "kim-hardware.h"
#include "boardhardware.h"

/*
Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 ***** Please set the number of devices you have *****
 But the maximum default of 8 MAX72XX wil also work.
 */
const int delaytime = 150;

byte dig[19] = {
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

#if BOARD_LED_I2C
LedControl lc = LedControl(LED_I2C_DATA, LED_I2C_CLK, LED_I2C_CS, 2);
void init_display()
{
    lc = LedControl(LED_I2C_DATA, LED_I2C_CLK, LED_I2C_CS, 2);

    int devices = lc.getDeviceCount();
    for (int address = 0; address < devices; address++)
    {
        /*The MAX72XX is in power-saving mode on startup*/
        lc.shutdown(address, false);
        lc.setIntensity(address, 8);
        lc.clearDisplay(address);
    }

    byte logo[]{
        0b01110111, // A
        0b00000101, // r
        0b00111101, // d
        0b00011100, // u
        0b00010000, // i
        0b00010101, // n
        0b00011101  // o
    };

    int l = 7;
    for (int i = 0; i < l; i++)
    {
        lc.setRow(0, l - i, logo[i]);
        delay(delaytime);
    }

    lc.clearDisplay(0);
    delay(delaytime);

    for (size_t i = 0; i < sizeof dig / sizeof dig[0]; i++)
    {
        lc.setRow(0, 0, dig[i]);
        delay(delaytime);
    }
}

void driveLEDs()
{
    int ledNo;
    int byt, i;
    int out;
    static int xlate[] = {7, 6, 5, 4, 2, 1};
    static char lastThreeHex[] = {0xF, 0xF, 0xF, 0xF, 0xF, 0xF};

    for (byt = 0; byt < 3; byt++)
    {
        for (i = 0; i < 2; i++)
        {
            ledNo = byt * 2 + i;
            char bcd = threeHex[byt][i];
            if (bcd != lastThreeHex[ledNo])
            {
                out = dig[(int)threeHex[byt][i]];
                lc.setRow(0, xlate[ledNo], out);
                lastThreeHex[ledNo] = bcd;
            }
        }
    }
}
#endif

#if BOARD_WIRED_LED
void init_display()
{
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
#endif
