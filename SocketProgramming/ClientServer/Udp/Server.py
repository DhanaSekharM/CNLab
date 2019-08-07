import socket

port = 1234
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('127.0.0.1', port))

while True:
	data, address = server_socket.recvfrom(1024)
	print(data.decode())
	server_socket.sendto(b'ack', address)