# TCP Server + Client Project 1 
This is a TCP server-client program following the observer design pattern. In this program, I implement functionality for client objects and server objects. 

A server is created with several functions defined in ./tcp_server.h. For each client, a unique ID is assigned, based on which the server either generates even or odd random, unique numbers. 

These random numbers are then written into a client application file in the form of a linked list. Every 10 seconds, this list is sorted for the client currently connected. 

### Platforms Support
Both Linux and Mac with GCC are compatible. 

### Examples
The code runners are in the 'examples' directory. There are two main files, 'server_example.cpp' and 'client_example.cpp'. 

### Thread Safe 
The server is thread-safe, and can handle multiple clients at the same time, and remove dead clients resources automatically. 

## Quick start
To build the runners and start the server, open a terminal window and enter the following: 
    'cd Desktop/TCPServer && ./build.sh && cd && cd Desktop/TCPServer/build && ./tcp_server'

To start the runner for clients, open a terminal window and enter the following: 
    'cd Desktop/TCPServer/build && ./tcp_client'

### Observer Design Pattern 
Both the server and client are using the observer design pattern to register and handle events.
When registering to an event with a callback, you should make sure that:
- The callback is fast (not doing any heavy lifting tasks) because those callbacks are called from the context of the server or client. 
- No server / client function calls are made in those callbacks to avoid possible deadlock.