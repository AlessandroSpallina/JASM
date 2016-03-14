#!/usr/bin/python3

import os, sys
from commsock import CommSocket
from formatprint import Print
#from traceback import format_exc as backtrace 

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
	Print.info("[CONNECT] Connecting to: ",ipaddr,'\n')

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
		Print.info("Non-required authentication\n")
		return 0
	elif getmsg == "auth-not-requiredcheck-pwd-file":
		passwd = str(input("[AUTH] Type a password here: "))
		sendMsg = socketObject.sendMessage(passwd)
		return 0
	elif getmsg == "auth-required":
		passwd = str(input("[AUTH] Login password: "))
		sendMsg = socketObject.sendMessage(passwd)
		answer = socketObject.getMessage(256)
#		print(answer)
		if answer == "granted":
			Print.info("[AUTH]Passwd accepted!\n")
			return 0
		elif answer == "deniedretry":
			Print.warn("[AUTH] Wrong password!\n")
			passwdmsg = str(input("[AUTH] Retry: "))
			socketObject.sendMessage(passwdmsg)
			while True:
				sobject = socketObject.getMessage(256)
				if sobject == "deniedretry" or sobject == "retry":
					passwdmsg = str(input("[AUTH] Retry: "))
					socketObject.sendMessage(passwdmsg)
					Print.warn("[AUTH]Retry!\n")
				elif sobject == "authorized":
					Print.info("[AUTH]Authorized\n")
					return 0
	return -1

def cmdline(socketObject):
	if socketObject is None:
		return -3

	while True:
		getcmd = str(input("\n--> "))
		if getcmd == "quit":
			Print.info("Closing connection...\n")
			if socketObject.endConnection() == 0:
				Print.info("Exiting...\n")
				exit(0)
			else:
				Print.error("ERROR WHILE CLOSING CONNECTION!!\n")
				Print.error("EXITING WITH ERROR!!\n")
				exit(-1)

		socketObject.sendMessage(getcmd)
		resp = socketObject.getMessage(256)
		Print.resp(resp)

try:		
	argdict = argchk(sys.argv)
	host = argdict['connect']
	csock = CommSocket(host,__PORT__)
	banner(csock.getipaddr())

	initconn = initConnection(csock)
	if initconn == 0:
		Print.info("[CONNECT] Success - connected!\n")
	elif initconn == -2:
		Print.error("[CONNECT] Timeout - failed!\n")
		exit(-1)
	elif initconn == -1:
		Print.error("[CONNECT] General socket error - failed!\n")
		exit(-2)

	checks = checkThings(csock)
	if checks == -1:
		Print.error("Something wrong!!\n")
		exit(-3)
	elif checks == 0:
		Print.info("Access Granted!\n")
		cmdline(csock)

	endconn = closeConnection(csock)
	if endconn == 0:
		Print.info("[DISCONNECT] Success - disconnected!\n")
		exit(-4)
	elif endconn == -1:
		Print.error("[DISCONNECT] General socket error - failed!\n")
		exit(-5)
except KeyboardInterrupt:
	Print.info("Closing connection\n")
	if csock.endConnection() == 0:
		Print.info("Closed connection!\n")
	else:
		Print.error("ERROR WHILE CLOSING CONNECTION!\n")
		Print.error("EXITING WITH ERROR!!\n")
		exit(-1)

	Print.info("Bye bye!\n")
	exit(0)

if __name__ != "__main__":
	exit(-1)
	
