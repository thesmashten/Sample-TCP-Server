#pragma once

#include <string>
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>
#include <fstream>
#include "pipe_ret_t.h"
#include "client_event.h"
#include "file_descriptor.h"
#include <iostream>
#include <fstream>

struct Node{
    int data;
    struct Node* next;
    };

class Client {

    using client_event_handler_t = std::function<void(const Client&, ClientEvent, const std::string&)>;

private:
    std::string _ip = "";
    std::atomic<bool> _isConnected;
    std::thread * _receiveThread = nullptr;
    client_event_handler_t _eventHandlerCallback;

    void setConnected(bool flag) { _isConnected = flag; }

    void receiveTask();

    void terminateReceiveThread();

public:
    Client(int);
    FileDescriptor _sockfd;
    bool operator ==(const Client & other) const ;

    void setIp(const std::string & ip) { _ip = ip; }
    std::string getIp() const { return _ip; }

    void setEventsHandler(const client_event_handler_t & eventHandler) { _eventHandlerCallback = eventHandler; }
    void publishEvent(ClientEvent clientEvent, const std::string &msg = "");

    bool isConnected() const { return _isConnected; }

    void startListen();

    void send(const char * msg, size_t msgSize) const;

    void close();

    void print() const;

    Node* head = nullptr;
};


