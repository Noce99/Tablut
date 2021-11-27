import socket
s = socket.socket()
s.connect(("localhost", 5800))
# print(s.recv(2048))
white = "noce".encode()
s.send(len(white).to_bytes(4, byteorder='big'))
s.send(white)
print(s.recv(2048))
print(s.recv(2048))


move = '{"from":"d5","to":"d6","turn":"WHITE"}'.encode()
s.send(len(move).to_bytes(4, byteorder='big'))
s.send(move)
while True:
    pass
