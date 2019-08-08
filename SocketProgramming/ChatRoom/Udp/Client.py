import socket
import select
import sys

client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
address = str(sys.argv[1])
port = int(sys.argv[2])

while True:
    sockets = [sys.stdin, client_socket]
    read_sockets,write_socket, error_socket = select.select(sockets,[],[])
    for sock in read_sockets:
        if(sock == client_socket):
            message, addr = client_socket.recvfrom(1024)
            print(message.decode())
        else:
            message = input()
            client_socket.sendto(message.encode(), ('127.0.0.1', port))
            print('You: ', message)



client_socket.close()