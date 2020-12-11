#!/bin/sh

# Compile Emscripten
em++ -O2 -I $EMSCRIPTEN/system/include -s DISABLE_EXCEPTION_CATCHING=0 -c -std=c++11 base64.cpp
em++ -O2 -I $EMSCRIPTEN/system/include -s DISABLE_EXCEPTION_CATCHING=0 -c -std=c++11 decode-file.cpp
em++ -s DISABLE_EXCEPTION_CATCHING=0 -o decode-file.js base64.o decode-file.o

# Add Truebit runtime
node ~/emscripten-module-wrapper/prepare.js decode-file.js --file input.data --file output.data  --run --debug --out=bin --memory-size=25 --upload-ipfs
