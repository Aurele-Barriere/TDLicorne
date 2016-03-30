# TDLicorne
td arcsys

#What is this?
Implementing network for our previous game : 7colors

#What's done?
2 players can connect to a server, that launches the game.

#What has yet to be done?
Change 7colors so it can be played online.
Better encapsulation of data that has to be sent (for the game board).
Handling deconnections and such things.
Add spectate mode.
Handling timeouts (for strategies).


#First Issue
Right now, I didn't take care about the possible errors in send() and recv().
Which means sometimes, strings get cut and you get an empty string, or not what you wanted.

#Fixing the issue
This should be solved with send_verif and recv_verif functions.
At least, errors are properly handled.
However, I still have connection issues from time to time (even locally).

