import socket
s_client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#s_client.bind(('',12345))
s_client.sendto(b'Hola clase', ('localhost',12345))
data, serveraddr = s_client.recvfrom(4096)
print(str(serveraddr[0]) + " dice: ", data.decode())
s_client.close()