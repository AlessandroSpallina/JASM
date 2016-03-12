#!/usr/bin/python3

import os, sys
from commsock import CommSocket
from formatprint import Print

host = ""
__PORT__ = 9734

def argchk(opt):
	if opt is None:
		return -1

	if len(opt) > 3:
		return -2

	opt.pop(0)
	i = 0

	while i < len(opt) - 1:
		if opt[i] == "--connect":
			if opt[i + 1] != None:
				ipaddr = opt[i + 1]
		i = i + 1

	return {"connect":ipaddr}

def banner(ipaddr):
	print("** JASM Python Client **")
	print("[CONNECT] Connecting to: ",ipaddr)

def initConnection(socketObject):
	if socketObject is None:
		return -3

	connectval = socketObject.doConnect()
	if connectval == 0:
		return 0
	elif connectval == -2:
		return -2
	elif connectval == -1:
		return -1

def closeConnection(socketObject):
	if socketObject is None:
		return -3

	closeval = socketObject.endConnection()
	if closeval == 0:
		return 0
	elif closeval == -1:
		return -1

def checkThings(socketObject):
	if socketObject is None:
		return -3

## TOFIX ##
	getmsg = socketObject.getMessage(256)
#	print(getmsg)
	if getmsg == "auth-not-requirednochk-pwdfile":
		Print.info("Non-required authentication")
		return 0
	elif getmsg == "auth-not-requiredcheck-pwd-file":
		passwd = str(input("[Authentication] Type a password here: "))
		sendMsg = socketObject.sendMessage(passwd)
		return 0
	elif getmsg == "auth-required":
		passwd = str(input("[Authentication] Login password: "))
		sendMsg = socketObject.sendMessage(passwd)
		answer = socketObject.getMessage(256)
#		print(answer)
		if answer == "granted":
			Print.info("Passwd accepted!")
			return 0
		elif answer == "deniedretry":
			print("[Password Auth] Wrong password!")
			passwdmsg = str(input("[Authentication] Retry: "))
			socketObject.sendMessage(passwdmsg)
			while True:
				sobject = socketObject.getMessage(256)
				if sobject == "deniedretry" or sobject == "retry":
					passwdmsg = str(input("[Authentication] Retry: "))
					socketObject.sendMessage(passwdmsg)
					Print.warn("Retry!")
				elif sobject == "authorized":
					Print.info("Authorized")
					return 0
	return -1

def cmdline(socketObject):
	if socketObject is None:
		return -3

	while True:
		getcmd = str(input("--> "))
		socketObject.sendMessage(getcmd)
		resp = socketObject.getMessage(256)
		Print.response(resp)

argdict = argchk(sys.argv)
host = argdict['connect']
csock = CommSocket(host,__PORT__)
banner(csock.getipaddr())

initconn = initConnection(csock)
if initconn == 0:
	print("[CONNECT] Success - connected!")
elif initconn == -2:
	print("[CONNECT] Timeout - failed!")
	exit(-1)
elif initconn == -1:
	print("[CONNECT] General socket error - failed!")
	exit(-2)

checks = checkThings(csock)
if checks == -1:
	Print.error("Something wrong!!")
	exit(-3)
elif checks == 0:
	Print.info("Access Granted!")
	cmdline(csock)

endconn = closeConnection(csock)
if endconn == 0:
	print("[DISCONNECT] Success - disconnected!")
	exit(-4)
elif endconn == -1:
	print("[DISCONNECT] General socket error - failed!")
	exit(-5)

if __name__ != "__main__":
	exit -1
	

