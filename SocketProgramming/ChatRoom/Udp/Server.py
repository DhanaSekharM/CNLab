import socket
import threading

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

port = 9877
address = ('127.0.0.1', port)
server_socket.bind(address)
clients = []

def handle_clients(address, message):
    server_socket.sendto('Chat Room'.encode(), address)
    while True:
        # message, addr = server_socket.recvfrom(1024).decode()
        if message:
            print(address[0], ': ', message)
            message_to_send = address[0] + ': ' + message
            for client in clients:
                if client != addr :
                    client.sendto(message_to_send.encode())


while True:
    data, addr = server_socket.recvfrom(1024)
    clients.append(addr)
    # print(addr, ' connected')
    threading.Thread(target=handle_clients, args=(addr, data)).start()




