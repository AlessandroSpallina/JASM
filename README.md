### JASM

JustAnotherSystemMonitor

*What is this?*

Just Another System Monitor is a system monitoring daemon which interfaces with CLI and GUI, it wants to be hackable and modular, you can build modules for JASM easily... Wait until we get a functional monitoring monitor.

*Would you like to contribute?*

Anyone is accepted! Join us!

*Supported Operating System(s)?*

Unix-like operating systems are supported, but we are planning to use GNU C Libraries and Linux kernel APIs

*Noobs: how to compile?*

Just type make, the output directory is bin/ :)

*Mmh... Ok, other make rules?*

 - clean (cleans object files and other things)
 - cleanbin (cleans bin/ directory except placeholder to avoid bin directory gets deleted from git when staging)

*Compiler*

We use GCC with -O2 -pipe -Wall -std=c11 flags which can be changed inside Makefiles in cli/ and core/ changing CFLAGS

*Nerdy*

We use C11

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

*[INFO][DEVEL/STAGE]: module_logsender... development started*

Few days and we will release first module :)

### Dependencies

 * gtk+2

 (Not now)
 * libvncserver
 * libcpuid
 ...

### Platform

 * JASM Core: We think 70+% is completed
 * JASM CLI: 100% completed, fully working
 * JASM GTK2: Not started yet
 * JASM on Android: Not started yet
 * Getters/Modules: before, the core, after getters/modules

### Files
 * $HOME/.jasm_config: JASM Configuration file *IMPLEMENTING* {*WIP*}

### Licensing

This project is licensed under the GPL version 3, this is free software, you can get it, redistribuite, share and more!
Thanks to Free Software! Avoiding proprietary software! See LICENSE to get more infos about this license

### Developers

 * Alessandro Spallina: Main founder of this project [github.com/AlessandroSpallina]
 * Stefano Belli: Makefiles, fixes, READMEs (including this :) ) [github.com/StefanoBelli]
