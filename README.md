# base64
No-frills C++ implementation of a base64 encoder and decoder. Includes 
usage examples based on complimentary cleartext and ciphertext strings taken 
from https://en.wikipedia.org/wiki/Base64.

# Webassembly Branch
Webassembly is a temporary branch to develop a simple application that reads the base64 encoding of an Excel file from a text file and decodes it back into a binary Excel file. The application can be compiled using g++ into a native Windows of Linux executable. It can also be compiled using the Emscripten compiler, emcc, into webassembly that can run under node.js and Truebit OS.