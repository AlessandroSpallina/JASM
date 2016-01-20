#!/bin/sh

JASM_HEADERS="src/main/jasm/core/modules.h src/main/jasm/core/logger.h"
SYSTEMD_TARGET_DIR=/etc/systemd/system/
JASM_SYSTEMD_SERVICE=jasm.service

if [ $(id -u) -eq 0 ];
then
				[ -f bin/jasm ] && cp -v bin/jasm /usr/bin/jasm
				[ -f bin/jasmcli ] && cp -v bin/jasmcli /usr/bin/jasm
				[ -f $JASM_SYSTEMD_SERVICE ] && cp -v $JASM_SYSTEMD_SERVICE $SYSTEMD_TARGET_DIR
				[ -f lib/libjasm.so ] && cp -v lib/libjasm.so /usr/lib/libjasm.so 

				if [ -d /usr/include/jasm ]; 
				then
						cp -v $JASM_HEADERS /usr/include/jasm
				else
						mkdir -v /usr/include/jasm
						printf "\033[34m* Re-launch install.sh\033[0m\n"
						exit 0
				fi

				printf "* To enable jasm.service run:\n* systemctl enable jasm.service\n"
				printf "\033[32m* Copy the file \033[0m\033[34mdata/jconfig to /home/your_username/.jasm_config\033[0m\n"
else
				printf "* You must be root\n"
				exit 1
fi

exit 0