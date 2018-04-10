import socket
HOST='127.0.1.1'
PORT=7777
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
print('Connected, waiting for data...')

##TEST BLOCK
while 1:
    data=s.recv(2)
    decoded = data.decode()
    if decoded=='0':
        print('Value 0 received')
    elif decoded == '1':
        print('Value 1 received')
    elif decoded == 'q':
        break
    else:
        print('Input not recognised')

s.close()
