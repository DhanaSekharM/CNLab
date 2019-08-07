import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 1250
server_socket.bind(('127.0.0.1', port))
server_socket.listen(5)
while True:
	client_socket, address = server_socket.accept()
	print('Client: ', client_socket.recv(1024).decode())
	inp = input('You: ')
	client_socket.send(inp.encode())
	client_socket.close()