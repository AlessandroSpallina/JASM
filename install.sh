#!/bin/sh

SYSTEMD_TARGET_DIR=/etc/systemd/system/
JASM_SYSTEMD_SERVICE=jasm.service

if [ $(id -u) -eq 0 ];
then
				[ -f bin/jasm ] && cp -v bin/jasm /usr/bin/jasm
				[ -f $JASM_SYSTEMD_SERVICE ] && cp -v $JASM_SYSTEMD_SERVICE $SYSTEMD_TARGET_DIR

				printf "* To enable jasm.service run:\n* systemctl enable jasm.service\n"
				printf "\033[32m* Copy the file \033[0m\033[34mdata/jconfig to /home/your_username/.jasm_config\033[0m\n"
else
				printf "* You must be root\n"
				exit 1
fi

exit 0
