import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = 1224
address = ('127.0.0.1', port)
while True:
    inp = input('You:')
    client_socket.sendto(inp.encode(), address)
    data, address = client_socket.recvfrom(1024)
    print('Server: ', data.decode())
