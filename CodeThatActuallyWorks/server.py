import socket
import numpy as np
import cv2

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((socket.gethostname(),8007))
server_socket.listen(5)

cap = cv2.VideoCapture(0)

while(True):
    
    client_socket, address = server_socket.accept()

    # ----------------------------
    ret, frame = cap.read()
    #gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    # ---------------------------
    #print(frame)
    client_socket.sendall(frame)

cap.release()
cv2.destroyAllWindows()

    
