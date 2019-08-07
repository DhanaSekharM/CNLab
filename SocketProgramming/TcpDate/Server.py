import socket
import datetime

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 1239
server_socket.bind(('127.0.0.1', port))
server_socket.listen(5)

while True:
	client_socket, address = server_socket.accept()
	# data = client_socket.recv(1024)
	# print(data, 'requested')
	client_socket.send(str(datetime.datetime.now()).encode())
	client_socket.close()