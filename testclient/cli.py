#!/usr/bin/python3

from sockets import Socket

sck = Socket()

while(1):
    cmd = input("  >> ")
    sck.send(cmd)
    data = sck.recv()
    print(" >>> Server: \n{}".format(data))
