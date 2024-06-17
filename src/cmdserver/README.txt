A local TCP server to read and execute commands.

Simply connect to CmdServer::ADDRESS with an AF_LOCAL+SOCK_STREAM socket,
then send a shell command and close the socket. The server will run forever
until terminated.

Sent commands are executed with a blocking `system()` call.
Instead, consider appending " &" to run a command in the background.
