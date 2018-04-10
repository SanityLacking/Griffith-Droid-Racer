import socket
HOST='127.0.1.1'
PORT=7777
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print('Waiting to connect to client...')
conn, addr=s.accept()
s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
print ('Connected by', addr)

##TEST BLOCK
while True:
    data = input("Input test data: ")
    data_bytes = str.encode(data)
    conn.send(data_bytes)
    if (data == 'q'):
        break

s.close()
