import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = 1224
server_socket.bind(('127.0.0.1', port))

while True:
    data, address = server_socket.recvfrom(1024)
    print('Client: ', data.decode())
    inp = input('You: ')
    server_socket.sendto(inp.encode(), address)

