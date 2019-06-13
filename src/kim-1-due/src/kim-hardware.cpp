#ifdef TARGETWEB
#include "fake-progmen.h"
#else
#include <Arduino.h>
#endif

#include <stdint.h>

#include "builtin_display.h"
#include "cpu.h"
#include "keypad.h"
#include "kim-hardware.h"
#include "boardhardware.h"

#ifdef USE_EPROM
#include <EEPROM.h>
#endif

//#define CH2_SPEAKERPIN 9
#define WREG_OFFSET 0x0360

uint8_t curkey = 0;
uint8_t eepromProtect = 1; // default is to write-protect EEPROM
int blitzMode = 1;         // microchess status variable. 1 speeds up chess moves (and dumbs down play)
uint8_t useKeyboardLed = 0x01; // set to 0 to use Serial port, to 1 to use onboard keyboard/LED display.

char threeHex[3][2]; // LED display

// ---------- called from cpu.c ----------------------

uint8_t getAkey()
{
    return (curkey);
}

void clearkey()
{
    curkey = 0;
}

// getKIMkey() translates ASCII keypresses to codes the KIM ROM expects.
// note that, inefficiently, the KIM Uno board's key codes are first translated to ASCII, then routed through
// here just like key presses from the ASCII serial port are. That's inefficient but left like this
// for hysterical raisins.

uint8_t getKIMkey()
{
    //Serial.print('#');  Serial.print(curkey);  Serial.print('#');

    if (curkey == 0)
        return (0xFF); //0xFF: illegal keycode

    if ((curkey >= '0') & (curkey <= '9'))
        return (curkey - '0');
    if ((curkey >= 'A') & (curkey <= 'F'))
        return (curkey - 'A' + 10);
    if ((curkey >= 'a') & (curkey <= 'f'))
        return (curkey - 'a' + 10);

    if (curkey == VKEY_AD)   // ctrlA
        return (0x10);       // AD address mode
    if (curkey == VKEY_DA)   // ctrlD
        return (0x11);       // DA data mode
    if (curkey == VKEY_PLUS) // +
        return (0x12);       // step
    if (curkey == VKEY_GO)   // ctrlG
        return (0x13);       // GO
    if (curkey == VKEY_PC)   // ctrlP
        return (0x14);       // PC mode
    // curkey==ctrlR for hard reset (/RST) (KIM's RS key) is handled in main loop!
    // curkey==ctrlT for ST key (/NMI) is handled in main loop!
    return (curkey); // any other key, should not be hit but ignored by KIM
}

#ifdef USE_EPROM
uint8_t eepromread(uint16_t eepromaddress)
{
    return EEPROM.read(eepromaddress);
}

void eepromwrite(uint16_t eepromaddress, uint8_t bytevalue)
{
    if (eepromProtect == 0)
        EEPROM.write(eepromaddress, bytevalue);
    else
    {
        Serial.println(F("ERROR: EEPROM STATE IS WRITE-PROTECTED. HIT '>' TO TOGGLE WRITE PROTECT"));
        Serial.println(freeRam());
    }
}
#endif

uint8_t xkeyPressed() // just see if there's any keypress waiting
{
    return (curkey == 0 ? 0 : 1);
}

// check for out of RAM
int freeRam()
{
#ifdef CALC_RAM_SIZE
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
#else
    return -1;
#endif
}

// translate keyboard commands to KIM-I keycodes or emulator actions
void interpretkeys()
{
    // round 1: keys that always have the same meaning
    switch (curkey)
    {
    case VKEY_RS: // CtrlR = RS key = hardware reset (RST)
        reset6502();
        clearkey();
        //       Serial.print("RSet\n");
        break;

    case VKEY_ST: // CtrlT = ST key = throw an NMI to stop execution of user program
        nmi6502();
        clearkey();
        //       Serial.print("STop\n");
        break;

    case VKEY_SST_OFF: // SST off
        SSTmode = 0;
        clearkey();
        break;

    case VKEY_SST_ON: // SST on
        SSTmode = 1;
        clearkey();
        break;

    case VKEY_TOGGLE_SERIAL_MODE: // TAB pressed, toggle between serial port and onboard keyboard/display
        if (useKeyboardLed == 0)
        {
            useKeyboardLed = 1;
        }
        else
        {
            useKeyboardLed = 0;
        }
        reset6502();
        clearkey();
        break;

    case VKEY_TOGGLE_EPROM_WRITE: // Toggle write protect on eeprom
        if (eepromProtect == 0)
        {
            eepromProtect = 1;
            //          Serial.print(F("                                      Eeprom R/O     "));
        }
        else
        {
            eepromProtect = 0;
            //          Serial.print(F("                                      Eeprom R/W     "));
            //          delay(20);
        }
        clearkey();
        break;
    }
}

// =================================================================================================
// KIM Uno Board functions are bolted on from here
// =================================================================================================
#ifndef TARGETWEB

void setupUno()
{
    int i;
    // --------- initialse for scanning keyboard matrix -----------------
    // set columns to input with pullups
    for (i = 0; i < 8; i++)
    {
        pinMode(aCols[i], INPUT);     // set pin to input
        digitalWrite(aCols[i], HIGH); // turn on pullup resistors
    }
    // set rows to output, and set them High to be in Neutral position
    for (i = 0; i < 3; i++)
    {
        pinMode(aRows[i], OUTPUT);    // set pin to output
        digitalWrite(aRows[i], HIGH); // set to high
    }
    // --------- clear display buffer ------------------------------------
    for (i = 0; i < 3; i++)
    {
        threeHex[i][0] = threeHex[i][1] = 0;
    }
    //    Serial.println(F("KIM-UNO initialized."));
}

uint8_t parseChar(uint8_t n) //  parse keycode to return its ASCII code
{
    uint8_t c = 0;

    // KIM-I keys
    switch (n - 1)
    { //KIM Uno keyscan codes to ASCII codes used by emulator
    case 7:
        c = VKEY_0;
        break; //        note: these are n-1 numbers!
    case 6:
        c = VKEY_1;
        break; //
    case 5:
        c = VKEY_2;
        break; //
    case 4:
        c = VKEY_3;
        break; //
    case 3:
        c = VKEY_4;
        break; //
    case 2:
        c = VKEY_5;
        break; //
    case 1:
        c = VKEY_6;
        break; //
    case 0:
        c = VKEY_ST;
        break; // ST
    case 15:
        c = VKEY_7;
        break; //
    case 14:
        c = VKEY_8;
        break; //
    case 13:
        c = VKEY_9;
        break; //
    case 12:
        c = VKEY_A;
        break; //
    case 11:
        c = VKEY_B;
        break; //
    case 10:
        c = VKEY_C;
        break; //
    case 9:
        c = VKEY_D;
        break; //
    case 8:
        c = VKEY_RS;
        break; // RS
    case 23:
        c = VKEY_E;
        break; //
    case 22:
        c = VKEY_F;
        break; //
    case 21:
        c = VKEY_AD;
        break; // AD
    case 20:
        c = VKEY_DA;
        break; // DA
    case 19:
        c = VKEY_PLUS;
        break; // +
    case 18:
        c = VKEY_GO;
        break; // GO
    case 17:
        c = VKEY_PC;
        break; // PC
    case 16:
        c = (SSTmode == 0 ? VKEY_SST_ON : VKEY_SST_OFF);
        break; // 	SST toggle
    }

    return c;
}

void scanKeys()
{
    int led, row, col, noKeysScanned;
    static int keyCode = -1, prevKey = 0;
    static unsigned long timeFirstPressed = 0;

    // 0. disable driving the 7segment LEDs -----------------
    for (led = 0; led < 8; led++)
    {
        pinMode(ledSelect[led], INPUT); // set led pins to input
        // not really necessary, just to stop them
        // from driving either high or low.
        digitalWrite(ledSelect[led], HIGH); // Use builtin pullup resistors
    }


    // 1. initialise: set columns to input with pullups
    for (col = 0; col < 8; col++)
    {
        pinMode(aCols[col], INPUT);     // set pin to input
        digitalWrite(aCols[col], HIGH); // turn on pullup resistors
    }
    // 2. perform scanning
    noKeysScanned = 0;

    // #define DEBUG_KEYBOARD

    for (row = 0; row < 3; row++)
    {
        digitalWrite(aRows[row], LOW); // activate this row
        for (col = 0; col < 8; col++)
        {
            if (digitalRead(aCols[col]) == LOW)
            { // key is pressed
                keyCode = col + row * 8 + 1;
                if (keyCode != prevKey)
                {
#ifdef DEBUG_KEYBOARD
                    {
                        Serial.println();
                        Serial.print(" col: ");
                        Serial.print(col, DEC);
                        Serial.print(" row: ");
                        Serial.print(row, DEC);
                        Serial.print(" prevKey: ");
                        Serial.print(prevKey, DEC);
                        Serial.print(" KeyCode: ");
                        Serial.println(keyCode, DEC);
                    }
#endif

                    prevKey = keyCode;
                    curkey = parseChar(keyCode);

#ifdef DEBUG_KEYBOARD
                    Serial.print(" curkey: ");
                    Serial.print(curkey, DEC);
#endif

                    timeFirstPressed = millis(); //
                }
                else
                { // if pressed for >1sec, it's a ModeShift key
                    if ((millis() - timeFirstPressed) > 1000)
                    {                                // more than 1000 ms
                        if (keyCode == 9)            // it was RS button
                            curkey = '>';            // toggle eeprom write protect
                        timeFirstPressed = millis(); // because otherwise you toggle right back!
                    }
                }
            }
            else
                noKeysScanned++; // another row in which no keys were pressed
        }
        digitalWrite(aRows[row], HIGH); // de-activate this row
    }

    if (noKeysScanned == 24) // no keys detected in any row, 3 rows * 8 columns = 24. used to be 28.
        prevKey = 0;         // allows you to enter same key twice
} // end of function
#endif
