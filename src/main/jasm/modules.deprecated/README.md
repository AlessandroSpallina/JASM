# JASM Modules

*Please NOTE: This README is just a draft for now*

## How modules works? [Especially for client programmers]
To avoid confusion, we advise you to open new socket (by client-side), and once it will be connected, request JASM new module execution on new thread. User just sends command to the server. No output if all is correctly done.

## Modules logic
 * Client opens new socket, then
 * calls start*
 * JASM creates a new thread
 * JASM updates runtime-execution modules and it executes 'mainmodule' function

## Module functions
* void init_module (int fd) //contains instructions for initializing module, fd (integer) means the file descriptor
* void start_module (int sec) //contains instructions for starting module, sec (integer) means the update interval

Use statical functions as you want.
JASM handles threads.

## Client command to start/stop modules 
* start* -> initializes environment for new module execution
* stop* -> stops module
