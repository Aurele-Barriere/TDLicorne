# TD Sockets
td arcsys

#What is this?
Implementing network for our previous game : 7colors

2 players can connect to a server, that launches the game.
Observers can join in at any moment to watch the game.


# Compiling

Run $ make
Then to launch the server $./server portno

Where portno is the port number you want to launch the server on.



To launch players $./client portno hostname

Where portno is the same as the server.

You can choose 127.0.0.1 for hostname to play locally.



To launch observers $./observer portno hostname


# Dependencies

make

gcc

SDL2