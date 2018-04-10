import socket
import cv2
import numpy as np
import time

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('192.168.43.68', 2000))

while True:

    buf = clientsocket.recv(2332800)
    nparr = np.fromstring(buf, np.uint8)
    if len(nparr) == 2332800:
        nparr = np.reshape(nparr, (720, 1080, 3))

        cv2.imshow("image", nparr)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
