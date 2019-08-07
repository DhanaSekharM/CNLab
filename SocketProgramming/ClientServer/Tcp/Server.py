import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
port = 1025
server_socket.bind(('127.0.0.1', port))
server_socket.listen(5)
# print('whaat')

while True:
	client_socket, address = server_socket.accept()
	print('connected to ', address)
	client_socket.send('Ack'.encode())
	client_socket.close()