#pragma once

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <functional>
#include <cstring>
#include <map>
#include <errno.h>
#include <iostream>
#include <mutex>
#include "client.h"
#include "tcp_client.h"
#include "server_observer.h"
#include "pipe_ret_t.h"
#include "file_descriptor.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

class TcpServer {
private:
    FileDescriptor _sockfd; //used to keep track of the file descriptor of the server 
    struct sockaddr_in _serverAddress; 
    struct sockaddr_in _clientAddress;
    fd_set _fds;
    std::vector<server_observer_t> _subscribers;
    std::mutex _subscribersMtx;

    std::thread * _clientsRemoverThread = nullptr;
    std::atomic<bool> _stopRemoveClientsTask;
    
    void startSorting(std::vector<Client*> _clients);
    void publishClientMsg(const Client & client, const char * msg, size_t msgSize);
    void publishClientDisconnected(const std::string&, const std::string&);
    pipe_ret_t waitForClient(uint32_t timeout);
    void clientEventHandler(const Client&, ClientEvent, const std::string &msg);
    void removeDeadClients();
    void terminateDeadClientsRemover();

public:
    TcpServer();
    ~TcpServer();
    std::mutex _clientsMtx;
    pipe_ret_t start(int port, int maxNumOfClients = 10, bool removeDeadClientsAutomatically = true);
    void initializeSocket();
    void bindAddress(int port);
    void listenToClients(int maxNumOfClients);
    Client* acceptClient(uint timeout);
    void subscribe(const server_observer_t & observer);
    pipe_ret_t sendToAllClients(const char * msg, size_t size);
    pipe_ret_t sendToClient(const std::string & clientIP, const char * msg, size_t size);
    pipe_ret_t close();
    void printClients();
    int numClientsConnected; //used to increment number of clients server is connected to 
    std::vector<int> numbers; //used to ensure unique num across day for each client 
    std::map<std::string, Client*> myMap;
    void sort(std::vector<Client*> _clients);
    int generateNumber(int number);
    std::vector<Client*> _clients;
    void sortList(int ID, std::string clientFileName);
    static pipe_ret_t sendToClient(const Client & client, const char * msg, size_t size);
};

