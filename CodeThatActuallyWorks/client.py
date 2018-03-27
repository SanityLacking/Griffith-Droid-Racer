import socket
import numpy as np
import cv2
import pygame

#Create a var for storing an IP address:
ip = socket.gethostname()
timer = 0
clock = pygame.time.Clock()
while 1:
    if timer < 1:
        #Create a socket connection for connecting to the server:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((ip, 8007))
        #Recieve data from the server:
        data = client_socket.recv(1024000)
        data = np.fromstring(data, np.uint8)
        cv2.imshow('frame', data)
        #print(data)
        #Set the timer back to 30:
        timer = 30
    else:
        timer -= 1
    clock.tick(1000)
    
