# emcc dice-roll.c ../../src/kim-1-due/src/webmain.cpp -s LINKABLE=1 -s EXPORT_ALL=1 -s WASM=1 -O3 -D TARGETWEB -o index.js  

SOURCE="dice-roll.c"
SOURCE="${SOURCE} ../src/kim-1-due/src/webmain.cpp"
SOURCE="${SOURCE} ../src/kim-1-due/src/cpu.cpp"
SOURCE="${SOURCE} ../src/kim-1-due/src/builtin_display.cpp"
SOURCE="${SOURCE} ../src/kim-1-due/src/kim-hardware.cpp"

emcc ${SOURCE} -s EXPORTED_FUNCTIONS='["_websetup", "_webloop", "_injectkey"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -s WASM=1 -O3 -D TARGETWEB -o index.js

emrun --no_browser --port 8080 .
#python -m SimpleHTTPServer 9000
