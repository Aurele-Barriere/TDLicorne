all : server client

server :  server.c network.c defines.h
	gcc server.c network.c -o server

client : client.c network.c defines.h
	gcc client.c network.c -o client
