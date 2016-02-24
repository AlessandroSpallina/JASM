### JASM

*NEWS: Switch to CMake, read below for instructions*

*URGENT: JASMCLI marked as deprecated, will be replaced soon*

JustAnotherSystemMonitor

*What is this?*

Just Another System Monitor is a system monitoring daemon which interfaces with CLI and GUI, it wants to be hackable and modular, you can build modules for JASM easily... Wait until we get a functional monitoring monitor.

*Would you like to contribute?*

Anyone is accepted! Join us!

*Supported Operating System(s)?*

Unix-like operating systems are supported, but we are planning to use GNU C Libraries and Linux kernel APIs

*Noobs: how to compile?*

We just switched *CMake* build automation tool!
Configure this project almost everywhere!
Follow instructions below (as we suggest) 

*Compiler*

We use GCC with -O2 -pipe -Wall -std=c11 flags which can be changed inside Makefiles in cli/ and core/ changing CFLAGS

*Nerdy*

We use C11

### USE CMAKE (IMPORTANT)
 
 As you read above, we just switched to cmake, now you can configure this project even on Windows, but we will give you instruction for GNU/Linux (and almost UNIX and Unix-like OS)

 CMake is a build-automation tool, with it you can build project files (from XCode to VS) and Makefiles! (Cross-platform Make)

 Follow this instructions

 * Be sure you are using GCC
 ~~~
 $ export CC=/usr/bin/gcc
 ~~~
 You should change your shell's rc and add above line to the rcfile.
 If you are using a shell like fish (it is NOT POSIX compilant)
 ~~~
 $ set -x CC /usr/bin/gcc
 ~~~

 * Well, you are in JASM directory... Now?
 We already created a directory called "buildenv", CMakeFiles are configured to use it
 ~~~
 $ ls buildenv 
 ~~~
 *probably it is empty, if no output, then it is right*

 * Change directory, configure, compile
  Now it's time to compile!
  ~~~
  $ cd buildenv/
  ~~~
  ... configure via cmake
  ~~~
  $ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_INSTALL_PREFIX=/usr/local
  ~~~
  *Explaination: cmake checks for CMakeLists.txt in the '..' directory*
  * -DCMAKE_BUILD_TYPE=Relase : means that the build must be "Release", but you may want to use "-DCMAKE_BUILD_TYPE=Debug"
  * -DCMAKE_C_COMPILER=/usr/bin/gcc : tells cmake what compiler should use, it is not really necessary if you set CC=gcc (*INFO: for CXX, same thing but its CMAKE_CXX_COMPILER*)
  Then, we can launch make
  * -DCMAKE_INSTALL_PREFIX=/usr/local : tells cmake the install directory (Not necessary)
  *on UNIX is /usr/local by default, on WINDOWS C:\Program Files*
  (Note that you may change the systemd service file)
  ~~~
  $ make -j2
  ~~~
  Now make (-j2 = 2 jobs working...) does all the stuffs and you can find jasm to ../bin/jasm ; if you used the Debug: ../bin-debug/jasm
  You may want to know what happens in the backend
  ~~~
  $ make VERBOSE=1 -j2
  ~~~
  
  Now, go below to the install parameter, jumping instructions below(later)

 * Now, you may want to delete buildenv content, just use
 ~~~
 $ rm -rf *
 ~~~
 *Inside buildenv/*

 * You can also use clean rule
 ~~~
 $ make clean
 ~~~
 *Inside buildenv/*

### Versions

 * develstage: high-development stage, then we get 0.0.9, before getting 0.1.0

### Project Structure

 * bin/: where binaries are placed after compilation
 * data/: for now, we use data for logging (logging works only from bin/ [development stage :) ])
 * doc/: where documentation is placed, an italian one is availible (Markdown!).
 * src/: where sources are placed
 * Makefile: we are using make to handle project, easily hackable!
 * LICENSE: Licensed under GNU GPL v3 [Contains the LICENSE]
 * README.md: hey :)

### Modules / Getters

As I said before, JASM wants to be easily hackable, so we are providing tools to create new modules for this System Monitor:

 * Modules: time-functional stats/feature (such as CPU Usage) [src/main/jasm/modules]
 * Getters: non-time-functional stats/features (such as Machine nodename)

### Dependencies

 * Nothing

### Install

 *Inside buildenv/*
 ~~~
 make install
 ~~~
 *remember to copy data/jconfig to /home/username/.jasm_config*
 
 * then, take data/jconfig and put it to $HOME/.jasm_config
 * copy jasm.service to /etc/systemd/system/jasm.service
 
### Platform

 * JASM Core: Development 
 * JASM CLI: Current release marked as deprecated! Will be replaced soon...
 * JASM Qt: Development
 * JASM Swing: Starting...
 * JASM on Android: Not started yet
 * Getters/Modules: before, the core, after getters/modules

### Files
 * $HOME/.jasm_config: JASM Configuration file *IMPLEMENTING* {*WIP*}

### Branches

 On main repository we are going to create new branch "stable"
 * master : main-development branch
 * stable : stable releases (less bugs, more functions)
 
### Slack

 We use Slack platform to communicate 
 https://jasm-team.slack.com
 
### Licensing

This project is licensed under the GPL version 3, this is free software, you can get it, redistribuite, share and more!
Thanks to Free Software! Avoiding proprietary software! See LICENSE to get more infos about this license

### Developers

 * Alessandro Spallina: Main founder of this project [https://github.com/AlessandroSpallina]
 * Stefano Belli: main contributor [https://github.com/StefanoBelli]
 * Emanuele Pirruccio: contributor [https://github.com/etneo90]
 * Francesco Pistritto : contributor [https://github.com/piFra]