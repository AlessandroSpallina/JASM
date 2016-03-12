#!/usr/bin/python3

from sys import stdout
from sys import stderr 

class Print:

	@staticmethod
	def info(*args):
		pinfo = "[INFO] "
		for i in args:
			pinfo = pinfo + str(i) + '\0'
		stdout.write(str(pinfo))

	@staticmethod
	def warn(*args):
		pwarn = "[WARN] "
		for i in args:
			pwarn = pwarn + str(i) + '\0'
		stdout.write(str(pwarn))

	@staticmethod
	def error(*args):
		perr = "[ERROR] "
		for i in args:
			perr = perr + str(i) + '\0'
		stderr.write(str(perr))

	@staticmethod
	def resp(*args):
		rsrv = "[RESPONSE] "
		for i in args:
			rsrv = rsrv + str(i) + '\0'
		stdout.write(str(rsrv))

