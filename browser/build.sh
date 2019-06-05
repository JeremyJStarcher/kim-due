# emcc dice-roll.c ../../src/kim-1-due/src/webmain.cpp -s LINKABLE=1 -s EXPORT_ALL=1 -s WASM=1 -O3 -D TARGETWEB -o index.js  

# Make a local copy so we can access it via our source maps
rm -rf csrc
cp ../src/kim-1-due/src csrc -Rv

SOURCE="dice-roll.c"
SOURCE="${SOURCE} ./csrc/webmain.cpp"
SOURCE="${SOURCE} ./csrc/cpu.cpp"
SOURCE="${SOURCE} ./csrc/builtin_display.cpp"
SOURCE="${SOURCE} ./csrc/kim-hardware.cpp"

LEVEL="-O0 -g4"
#LEVEL="-O3"

emcc ${SOURCE} -s EXPORTED_FUNCTIONS='["_websetup", "_webloop", "_injectkey"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' -s WASM=1 ${LEVEL} -D TARGETWEB -o index.js

emrun --no_browser --port 8080 .
#python -m SimpleHTTPServer 9000
