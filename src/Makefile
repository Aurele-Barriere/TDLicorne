all : server client

server :  server.c network.c
	gcc server.c network.c -o server

client : client.c network.c
	gcc client.c network.c -o client
