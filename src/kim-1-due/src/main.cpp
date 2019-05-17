#include <Arduino.h>

#include "cpu.h"
#include "lcd.h";
#include "kim-hardware.h"

void setup()
{

    Serial.begin(9600);
    Serial.println();

    setupUno();

    reset6502();
    initKIM(); // Enters 1c00 in KIM vectors 17FA and 17FE. Might consider doing 17FC as well????????
    loadTestProgram();

    Serial.print(F("V2019-05-12 FREEMEM=")); // just a little check, to avoid running out of RAM!
    Serial.println(freeRam());

    init_lcd();
}

void loop()
{
    exec6502(100); //do 100 6502 instructions

    if (Serial.available())
    {
        curkey = Serial.read() & 0x7F;
        interpretkeys();
    }

    scanKeys();
    if (xkeyPressed() != 0) //KIM Uno board input?
        interpretkeys();
}
