# base64
No-frills C++ implementation of a base64 encoder and decoder. Includes 
usage examples based on complimentary cleartext and ciphertext strings taken 
from https://en.wikipedia.org/wiki/Base64.

# Webassembly Branch
Webassembly is a branch to illustrate compiling and submitting a task to the Truebit OS. While this tool both codes and decodes base64, the application in this branch only decodes. The application can be compiled using g++ into a native Windows or Linux executable. It can also be compiled using the Emscripten compiler, emcc, into webassembly that can run under node.js and Truebit OS.

To compile this project to WASM using the Emscripten toolchain in the Truebit Docker container:
* Clone this branch into a project directory named base64 on the Truebit Docker container's host
* From the host command line:
    * change to the parent of your base64 project directory
    * copy the base64 project folder and all of its contents into the Truebit samples folder in the Truebit Docker container
PS C:\Users\me\projects> **docker cp ./base64 test-net1:/truebit-eth/wasm-ports/samples**
* From the Truebit Docker container command line:
    * change to the base64 project directory  
root@docker-desktop:/# **cd /truebit-eth/wasm-ports/samples/base64**
    * copy base64.json to the base truebit-eth folder  
root@docker-desktop:/truebit-eth/wasm-ports/samples/base64# **cp base64.json /truebit-eth/**
    * run compile script  
root@docker-desktop:/truebit-eth/wasm-ports/samples/base64# **sh compile.sh**
    * copy globals.wasm in the bin subfolder to the base base64 folder renaming it to task.wasm  
root@docker-desktop:/truebit-eth/wasm-ports/samples/base64# **cp bin/globals.wasm task.wasm**
* From a truebit-os session command line in the same Truebit Docker container:
    * start a solver if there is none already using an account with a solver license  
truebit-os:> **start solve -a 1**
    * start a verifier if there is none already  
truebit-os:> **start verify -a 2**
    * submit base64 task  
truebit-os:> **task -f base64.json submit -a 0** 
