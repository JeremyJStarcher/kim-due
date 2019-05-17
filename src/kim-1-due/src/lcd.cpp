#include "LedControl.h"
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

void init_lcd()
{
    LedControl lc = LedControl(74, 76, 7, 2);

    //we have already set the number of devices when we created the LedControl
    int devices = lc.getDeviceCount();
    //we have to init all devices in a loop
    for (int address = 0; address < devices; address++)
    {
        /*The MAX72XX is in power-saving mode on startup*/
        lc.shutdown(address, false);
        /* Set the brightness to a medium values */
        lc.setIntensity(address, 8);
        /* and clear the display */
        lc.clearDisplay(address);
    }

    lc.setChar(0, 0, 'a', false);
    delay(delaytime);
    lc.setRow(0, 0, 0x05);
    delay(delaytime);
    lc.setChar(0, 0, 'd', false);
    delay(delaytime);
    lc.setRow(0, 0, 0x1c);
    delay(delaytime);
    lc.setRow(0, 0, B00010000);
    delay(delaytime);
    lc.setRow(0, 0, 0x15);
    delay(delaytime);
    lc.setRow(0, 0, 0x1D);
    delay(delaytime);
    lc.clearDisplay(0);
    delay(delaytime);
}