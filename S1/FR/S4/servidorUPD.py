import socket
s_server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s_server.bind(('',12345))
data, clientaddr = s_server.recvfrom(4096)
print(str(clientaddr[0]) + " dice: ", data.decode())
s_server.sendto(b'Bienvenido a clase', (clientaddr))
s_server.close()