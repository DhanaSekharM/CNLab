import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = 1234
client_socket.sendto(b'hs', ('127.0.0.1', port))
data, address = client_socket.recvfrom(1024)
print(data.decode())