import socket
import select
import sys

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
address = str(sys.argv[1])
port = int(sys.argv[2])
client_socket.connect(('127.0.0.1', port))

while True:
    sockets = [sys.stdin, client_socket]
    read_sockets,write_socket, error_socket = select.select(sockets,[],[])
    for sock in read_sockets:
        if(sock == client_socket):
            message = client_socket.recv(1024).decode()
            print(message)
        else:
            message = input()
            client_socket.send(message.encode())
            print('You: ', message)



client_socket.close()