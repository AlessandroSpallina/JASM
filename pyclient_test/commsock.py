#!/usr/bin/python3

import socket
import threading

class CommSocket:
	def __init__(self,_ipaddr,_port):
		self.ipaddr = _ipaddr 
		self.port = _port
		self.tcp_sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

	def doConnect(self):
		try:
			self.tcp_sock.connect((self.ipaddr,self.port))
		except socket.timeout:
			return -2
		except socket.error:
			return -1
		return 0

	def endConnection(self):
		try:
			self.tcp_sock.close()
		except socket.error:
			return -1
		return 0

	def sendMessage(self,_msg):
		_msg_bytes = str.encode(_msg)
		try:
			self.tcp_sock.send(_msg_bytes)
		except socket.error:
			return -1
		return 0

	def getMessage(self,bufsz):
		try:
			sock_msg = self.tcp_sock.recv(bufsz)
		except socket.error:
			return -1
		return str(sock_msg.decode("utf-8")) 

	def getipaddr(self):
		return self.ipaddr

	def getport(self):
		return self.port  


if __name__ == "__main__":
	exit -1