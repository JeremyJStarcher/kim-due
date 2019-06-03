#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <emscripten/emscripten.h>

// The code inside the main will be executed once the WASM module loads.
int main(int argc, char ** argv) {
    printf("WebAssembly module loaded\n");
}

// Simple C function that returns a number between 1 and 6.
int EMSCRIPTEN_KEEPALIVE roll_dice() {
    srand ( time(NULL) );

    uint8_t r = rand() % 6 + 1;

    printf("Rolled: %d\n",r);

    EM_ASM({
		    alert("hi");
		    });

    // note that you have to specify the return type after EM_ASM_
    int result = EM_ASM_INT({
        console.log("(3) I received two parameters! They are:", $0, $1);
        console.log("Let's return their sum!");
        return sum($0, $1);

        function sum(a, b){
            return a + b;
        }
    }, 13, 10);		

    return r;
}

