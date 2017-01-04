import socket

class Socket:
    def __init__(self,ipaddr="127.0.0.1",port=9734):
        self.ipaddr=ipaddr
        self.port=port
        self.sck=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.sck.connect((ipaddr,port))
    
    def send(self,data):
        defdata="Data-Size: {}\n\n{}".format(len(data),data)
        return self.sck.sendall(bytes(defdata.encode('utf-8')))

    def recv(self,bufsize=1024):
        return self.sck.recv(bufsize).decode("utf-8","replace")

    def close(self):
        self.sck.close()

if __name__ == '__main__':
    exit(1)
