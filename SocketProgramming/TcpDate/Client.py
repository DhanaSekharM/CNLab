import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 1239
client_socket.connect(('127.0.0.1', port))
print('Date: ', client_socket.recv(1024).decode())
client_socket.close()