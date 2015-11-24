#!/bin/sh

SYSTEMD_TARGET_DIR=/etc/systemd/system/
JASM_SYSTEMD_SERVICE=jasm.service

if [ $(id -u) -eq 0 ];
then
				[ -f bin/jasm ] && cp -v bin/jasm /usr/bin/jasm 
				[ -f bin/jasmcli ] && cp -v bin/jasmcli /usr/bin/jasmcli
				[ -f bin/jasmgtk ] && cp -v bin/jasmgtk /usr/bin/jasmgtk
				[ -f $JASM_SYSTEMD_SERVICE ] && mv -v $JASM_SYSTEMD_SERVICE $SYSTEMD_TARGET_DIR 
				printf "* To enable jasm.service run:\n* systemctl enable jasm.service\n"

else
				printf "* You must be root\n"
fi

