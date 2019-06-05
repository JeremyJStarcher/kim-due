#ifdef TARGETWEB

#include "cpu.h"
#include "builtin_display.h"
#include "kim-hardware.h"
#include <emscripten/emscripten.h>

#include <stdio.h>
extern "C"
{
    void EMSCRIPTEN_KEEPALIVE websetup()
    {
        reset6502();
        initKIM(); // Enters 1c00 in KIM vectors 17FA and 17FE. Might consider doing 17FC as well????????
        loadTestProgram();

        init_display();
    }

    void webloop()
    {
        puts("Webloop");
        exec6502(100); //do 100 6502 instructions

#if 0
    if (Serial.available())
    {
        curkey = Serial.read() & 0x7F;
        interpretkeys();
    }

    scanKeys();
    if (xkeyPressed() != 0)
    {
        interpretkeys();
    }
#endif
    }
}

#endif