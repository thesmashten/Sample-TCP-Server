///////////////////////////////////////////////////////////
/////////////////////SERVER RUNNER////////////////////////
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
server_observer_t observer1, observer2;

// observer callback. will be called for every new message received by clients
// with the requested IP address
// this is the callback for the even server 
void onIncomingMsg1(const std::string &clientIP, const char * msg, size_t size) {
   std::string msgStr = msg;
   int ID = std::stoi(msg);
   Client* client = server._clients[ID];
   int value = server.generateNumber(ID);
   Node* head = client->head;
   std::ofstream clientFile;
   std::string clientFileName;
   if (ID % 2 == 0){
       clientFileName = "(EVEN) CLIENT ID #: " + std::to_string(ID);
   }
   else{
       clientFileName = "(ODD) CLIENT ID #: " + std::to_string(ID);
   }
   clientFile.open(clientFileName);
   while (head->next != nullptr){
       clientFile << head->data << "->";
       head = head->next;
   }
   if (head != nullptr){
        clientFile << head->data;
   }
   clientFile.close();
   if (ID % 2 == 0){
       std::cout << "\nClient with ID " << ID << " requested a new unique even number for the day." << "\n";
   }
   else{
       std::cout << "\nClient with ID " << ID << " requested a new unique odd number for the day." << "\n";
   }
   sleep(5);
   server.sortList(ID, clientFileName);
   std::string theValue = std::to_string(value);
   server.sendToClient(*server._clients[ID], theValue.c_str(), theValue.size());
}

 
// observer callback. will be called when client disconnects from even server
void onClientDisconnected(const std::string &ip, const std::string &msg) {
   std::cout << "Client: " << ip << " disconnected. Reason: " << msg << "\n";
}

void evenServer(){
   // start server on port 65123
   int port = 65123;
   int maxClients = 20;
   bool removeClients = true;
   pipe_ret_t startRet = server.start(port, maxClients, removeClients);
   if (startRet.isSuccessful()) {
       std::cout << "\n\nSERVER SETUP SUCCEEDED WITH PORT NUMBER: " << port << "\n";
   } else {
       std::cout << "\nSERVER SETUP FAILED: " << startRet.message() << "\n";
   }
 
   // configure and register observer1
   observer1.incomingPacketHandler = onIncomingMsg1;
   observer1.disconnectionHandler = onClientDisconnected;
   server.subscribe(observer1);
 
   while (true){
       try {
       std::cout << "\nSERVER WAITING FOR INCOMING CLIENT...\n";
       Client* client = server.acceptClient(0);
       std::cout << "\n\n<<Even server accepted new client with IP: " << client->getIp() << ">>\n" <<
                 "== updated list of accepted clients ==" << "\n";
       server.printClients();
       observer1.wantedIP = client->getIp();
   } 
   catch (const std::runtime_error &error) {
       std::cout << "Accepting client failed: " << error.what() << "\n";
    }
   }
}


 
int main()
{
    std::thread even(evenServer);
    even.join();
   return 0;
}
 
#endif

