import socket

while True:
	client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	port = 1250
	client_socket.connect(('127.0.0.1', port))
	inp = input('You: ')
	client_socket.send(inp.encode())
	print('Client :', client_socket.recv(1024).decode())
	client_socket.close()