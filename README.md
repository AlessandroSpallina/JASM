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

### Project Structure

 - bin/: where binaries are placed after compilation
 - data/: for now, we use data for logging (logging works only from bin/ [development stage :) ])
 - doc/: where documentation is placed, an italian one is availible (Markdown!).
 - src/: where sources are placed
 - Makefile: we are using make to handle project, easily hackable! 
 - LICENSE: Licensed under GNU GPL v3 [Contains the LICENSE]
 - README.md: hey :)

### Licensing

This project is licensed under the GPL version 3, this is free software, you can get it, redistribuite, share and more! 
Thanks to Free Software! Avoiding proprietary software! See LICENSE to get more infos about this license

### Source tree
 
 - src/
   -> main/ 
	  -> jasm/
		 -> core/
		 -> modules/
		-> interfacing/
		 -> cli/
		 -> gui/
	 -> test/

### Developers

 - Alessandro Spallina: Main founder of this project [github.com/AlessandroSpallina]
 - Stefano Belli: Makefiles, fixes, READMEs (including this :) ) [github.com/StefanoBelli]

