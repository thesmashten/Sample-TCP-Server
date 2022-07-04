///////////////////////////////////////////////////////////
/////////////////////SERVER EXAMPLE////////////////////////
///////////////////////////////////////////////////////////
 
#ifdef SERVER_EXAMPLE
 
#include <iostream>
#include <csignal>
#include <fstream>
#include "../include/tcp_server.h"
#include <string>
 
 
// declare the server
TcpServer server;
 
// declare a server observer which will receive incomingPacketHandler messages.
// the server supports multiple observers
server_observer_t observer1;
 
// observer callback. will be called for every new message received by clients
// with the requested IP address
 
void sortList(int ID){
   Client* client = server._clients[ID];
   Node* head = client->head;
   if (head == nullptr || head->next == nullptr){
       return;
   }
   for (head; head->next != nullptr; head = head->next){
       for (Node* sel = head->next; sel != nullptr; sel = sel->next){
           if (head->data > sel->data){
                std::swap(head->data, sel->data);
               }
           }
       }
   client = server._clients[ID];
   head = client->head;
   std::ofstream clientFile;
   std::string clientFileName = "client " + std::to_string(ID);
   clientFile.open(clientFileName);
   while (head->next != nullptr){
       clientFile << head->data << "->";
       head = head->next;
   }
   if (head != nullptr){
       clientFile << head->data;
   }
    clientFile.close();
    sleep(10);
   }
 
void onIncomingMsg1(const std::string &clientIP, const char * msg, size_t size) {
   std::string msgStr = msg;
   int number = std::stoi(msg);
   Client* client = server._clients[number];
   int value = server.generateNumber(number);
   Node* head = client->head;
   std::ofstream clientFile;
   std::string clientFileName = "client " + std::to_string(number);
   clientFile.open(clientFileName);
   while (head->next != nullptr){
       clientFile << head->data << "->";
       head = head->next;
   }
   if (head != nullptr){
        clientFile << head->data;
   }
   clientFile.close();
   sortList(number);
   std::cout << "Observer1 got client msg: " << msgStr << "\n";
}
 
    
 
 
// observer callback. will be called when client disconnects
void onClientDisconnected(const std::string &ip, const std::string &msg) {
   std::cout << "Client: " << ip << " disconnected. Reason: " << msg << "\n";
}
 
// accept a single client.
// if you wish to accept multiple clients, call this function in a loop
// (you might want to use a thread to accept clients without blocking)
Client* acceptClient() {
   try {
       std::cout << "waiting for incoming client...\n";
       Client* client = server.acceptClient(0);
       std::cout << "accepted new client with IP: " << client->getIp() << "\n" <<
                 "== updated list of accepted clients ==" << "\n";
       server.printClients();
       return client;
   } catch (const std::runtime_error &error) {
       std::cout << "Accepting client failed: " << error.what() << "\n";
   }
   return NULL;
}
 
void printMenu() {
   std::cout << "\n\nselect one of the following options: \n" <<
             "1. send all clients a message\n" <<
             "2. print list of accepted clients\n" <<
             "3. send message to a specific client\n" <<
             "4. close server and exit\n";
}
 
int getMenuSelection() {
   int selection = 0;
   std::cin >> selection;
   if (!std::cin) {
       throw std::runtime_error("invalid menu input. expected a number, but got something else");
   }
   std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
   return selection;
}
 
/**
* handle menu selection and return true in case program should terminate
* after handling selection
*/
bool handleMenuSelection(int selection) {
   static const int minSelection = 1;
   static const int maxSelection = 4;
   if (selection < minSelection || selection > maxSelection) {
       std::cout << "invalid selection: " << selection <<
                 ". selection must be b/w " << minSelection << " and " << maxSelection << "\n";
       return false;
   }
   switch (selection) {
       case 1: { // send all clients a message
           std::string msg;
           std::cout << "type message to send to all connected clients:\n";
           getline(std::cin, msg);
           pipe_ret_t sendingResult = server.sendToAllClients(msg.c_str(), msg.size());
           if (sendingResult.isSuccessful()) {
               std::cout << "sent message to all clients successfully\n";
           } else {
               std::cout << "failed to sent message: " << sendingResult.message() << "\n";
           }
           break;
       }
       case 2: { // print list of accepted clients
           server.printClients();
           break;
       }
       case 3: { // send message to a specific client
           std::cout << "enter client IP:\n";
           std::string clientIP;
           std::cin >> clientIP;
           std::cout << "enter message to send:\n";
           std::string message;
           std::cin >> message;
           pipe_ret_t result = server.sendToClient(clientIP, message.c_str(), message.size());
           if (!result.isSuccessful()) {
               std::cout << "sending failed: " << result.message() << "\n";
           } else {
               std::cout << "sending succeeded\n";
           }
           break;
       };
       case 4: { // close server
           pipe_ret_t sendingResult = server.close();
           if (sendingResult.isSuccessful()) {
               std::cout << "closed server successfully\n";
           } else {
               std::cout << "failed to close server: " << sendingResult.message() << "\n";
           }
           return true;
       }
       default: {
           std::cout << "invalid selection: " << selection <<
                     ". selection must be b/w " << minSelection << " and " << maxSelection << "\n";
       }
   }
   return false;
}
 
int main()
{
   // start server on port 65123
   pipe_ret_t startRet = server.start(65123);
   if (startRet.isSuccessful()) {
       std::cout << "Server setup succeeded\n";
   } else {
       std::cout << "Server setup failed: " << startRet.message() << "\n";
       return EXIT_FAILURE;
   }
 
   // configure and register observer1
   observer1.incomingPacketHandler = onIncomingMsg1;
   observer1.disconnectionHandler = onClientDisconnected;
   server.subscribe(observer1);
 
   while (true){
       Client* client = acceptClient();
       observer1.wantedIP = client->getIp();
   }
   return 0;
}
 
#endif

