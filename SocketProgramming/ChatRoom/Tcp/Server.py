import socket
import threading

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

port = 9876
address = ('127.0.0.1', port)
server_socket.bind(address)
server_socket.listen(5)
clients = []

def handle_clients(client_socket, address):
    client_socket.send('Chat Room'.encode())
    while True:
        message = client_socket.recv(1024).decode()
        if message:
            print(address[0], ': ', message)
            message_to_send = address[0] + ': ' + message
            for client in clients:
                if client != client_socket :
                    client.send(message_to_send.encode())


while True:
    client_socket, addr = server_socket.accept()
    clients.append(client_socket)
    print(addr, ' connected')
    threading.Thread(target=handle_clients, args=(client_socket, addr)).start()




