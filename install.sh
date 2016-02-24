#!/bin/sh

PREFIX=$1
SYSTEMD_TARGET_DIR=/etc/systemd/system/
JASM_SYSTEMD_SERVICE=jasm.service

if [ $# -eq 1 ];
then
		echo "Prefix: ${PREFIX}"
		echo "Path: ${PREFIX}/bin/jasm"
elif [[ $# > 1 ]] || [[ $# < 1 ]];
then
		echo "You need to specify destdir: "
		echo "$0 <prefix>"
		exit 2
fi

if [ $(id -u) -eq 0 ];
then
				[ -f bin/jasm ] && cp -v bin/jasm ${PREFIX}/bin/jasm || exit 1
				[ -f $JASM_SYSTEMD_SERVICE ] && cp -v $JASM_SYSTEMD_SERVICE $SYSTEMD_TARGET_DIR || exit 2

				printf "* To enable jasm.service run:\n* systemctl enable jasm.service\n"
				printf "\033[32m* Copy the file \033[0m\033[34mdata/jconfig to /home/your_username/.jasm_config\033[0m\n"
else
				printf "* You must be root\n"
				exit 1
fi

exit 0
