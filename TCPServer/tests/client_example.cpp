///////////////////////////////////////////////////////////
/////////////////////CLIENT EXAMPLE////////////////////////
///////////////////////////////////////////////////////////

#ifdef CLIENT_EXAMPLE

#include <iostream>
#include <csignal>
#include "../include/tcp_client.h"

TcpClient* client = new TcpClient();

// on sig_exit, close client
void sig_exit(int s)
{
	std::cout << "Closing client...\n";
	pipe_ret_t finishRet = client->close();
	if (finishRet.isSuccessful()) {
		std::cout << "Client closed.\n";
	} else {
		std::cout << "Failed to close client.\n";
	}
	exit(0);
}

// observer callback. will be called for every new message received by the server
void onIncomingMsg(const char * msg, size_t size) {
	std::cout << "Client # " << client->getID() << " got this unique number from the server: " << msg << "\n";
}


// observer callback. will be called when server disconnects
void onDisconnection(const pipe_ret_t & ret) {
	std::cout << "Server disconnected: " << ret.message() << "\n";
}

// std::vector<TcpClient*> generateClients(){
//     std::vector<TcpClient*> clients;
//     client_observer_t observer;
//     observer.wantedIP = "127.0.0.1";
// 	observer.incomingPacketHandler = onIncomingMsg;
// 	observer.disconnectionHandler = onDisconnection;
// 	client->subscribe(observer);
//     clients.push_back(client);
//     for (int i = 0; i < 9; i++){
//         client = new TcpClient();
//         client_observer_t observer;
//         observer.wantedIP = "127.0.0.1";
// 	    observer.incomingPacketHandler = onIncomingMsg;
// 	    observer.disconnectionHandler = onDisconnection;
// 	    client->subscribe(observer);
//         clients.push_back(client);
//     }
//     return clients; 
// }

// std::vector<TcpClient*> generateOdd(std::vector<TcpClient*> clients){
//     std::vector<TcpClient*> oddClients; 
//     for (int i = 0; i < clients.size(); i++){
//         TcpClient* client = clients[i];
//         if (client->getID() % 2 == 1){
//             oddClients.push_back(client);
//         }
//     }
//     return oddClients;
// }

// std::vector<TcpClient*> generateEven(std::vector<TcpClient*> clients){
//     std::vector<TcpClient*> evenClients; 
//     for (int i = 0; i < clients.size(); i++){
//         TcpClient* client = clients[i];
//         if (client->getID() % 2 == 0){
//             evenClients.push_back(client);
//         }
//     }
//     return evenClients;
// }

// void handleOddClients(std::vector<TcpClient*> oddClients) {
//     //register to SIGINT to close client when user press ctrl+c
//     pipe_ret_t connectRet; 
//     client_observer_t observer;
//     observer.wantedIP = "127.0.0.1";
// 	observer.incomingPacketHandler = onIncomingMsg;
// 	observer.disconnectionHandler = onDisconnection;
//     bool connected = false;
//     for (int i = 0; i < oddClients.size(); i++){
//         TcpClient* client = oddClients[i];
//         connectRet = client->connectTo(observer.wantedIP, 65123); //odd server
//         connected = connectRet.isSuccessful();
//         if (connected){
//             if (i % 2 == 0){
//                 std::cout<< "\nClient (even) has connected to the server.";
//                 std::cout<<" Client has an ID of " << i;
//             }
//             else{
//                 std::cout<<"\nClient (odd) has connected to the server.";
//                 std::cout<<" Client has an ID of " << client->getID();
//             }
//             client->printMenu();
//             int selection = client->getMenuSelection();
//             while (selection != 2){
//                 if (selection == 1){ //client requesting #
//                         std::string ID = std::to_string(client->getID());
//                         pipe_ret_t sendRet = client->sendMsg(ID.c_str(), ID.size());
//                         if (!sendRet.isSuccessful()) {
//                             std::cout << "\nFailed to send message: " << sendRet.message() << "\n";
//                         } 
//                         else {
//                             std::cout << "\nmessage was sent successfuly\n";
//                         }
//                         client->printMenu();
//                         selection = client->getMenuSelection();
//                     }
//             }
//             client->close();
//         }
//     }
// }


// void handleEvenClients(std::vector<TcpClient*> evenClients) {
//     //register to SIGINT to close client when user press ctrl+c
//     client_observer_t observer;
//     observer.wantedIP = "127.0.0.1";
// 	observer.incomingPacketHandler = onIncomingMsg;
// 	observer.disconnectionHandler = onDisconnection;
//     pipe_ret_t connectRet; 
//     bool connected = false;
//     for (int i = 0; i < evenClients.size(); i++){
//         TcpClient* client = evenClients[i];
//         connectRet = client->connectTo(observer.wantedIP, 65123); //odd server
//         connected = connectRet.isSuccessful();
//         if (connected){
//             if (i % 2 == 0){
//                 std::cout<< "\nClient (even) has connected to the server.";
//                 std::cout<<" Client has an ID of " << client->getID();
//             }
//             else{
//                 std::cout<<"\nClient (odd) has connected to the server.";
//                 std::cout<<"\nClient has an ID of " << client->getID();
//             }
//             client->printMenu();
//             int selection = client->getMenuSelection();
//             while (selection != 2){
//                 if (selection == 1){ //client requesting #
//                         std::string ID = std::to_string(client->getID());
//                         pipe_ret_t sendRet = client->sendMsg(ID.c_str(), ID.size());
//                         if (!sendRet.isSuccessful()) {
//                             std::cout << "\nFailed to send message: " << sendRet.message() << "\n";
//                         } 
//                         else {
//                             std::cout << "\nmessage was sent successfuly\n";
//                         }
//                         client->printMenu();
//                         selection = client->getMenuSelection();
//                     }
//                 client->close();
//             }
//         }
//     }
// }

// int main() {
//     //register to SIGINT to close client when user press ctrl+c
// 	signal(SIGINT, sig_exit);
//     std::vector<TcpClient*> clients = generateClients();
//     std::vector<TcpClient*> oddClients = generateOdd(clients);
//     std::vector<TcpClient*> evenClients = generateEven(clients);
//     std::thread even(handleEvenClients, evenClients);
//     std::thread odd(handleOddClients, oddClients);
//     even.join();
//     odd.join();
//     return 0;
// }



int main() {
    //register to SIGINT to close client when user press ctrl+c
	signal(SIGINT, sig_exit);

    // configure and register observer
    client_observer_t observer;
	observer.wantedIP = "127.0.0.1";
	observer.incomingPacketHandler = onIncomingMsg;
	observer.disconnectionHandler = onDisconnection;
	client->subscribe(observer); //for the first client subscribe to observer to display messages from server
    pipe_ret_t connectRet; 
	bool connected = false;
    for (int i = 0; i < 10; i++){
        connectRet = client->connectTo(observer.wantedIP, 65123); //odd server
        connected = connectRet.isSuccessful();
        if (connected){
            if (client->getID() % 2 == 0){
                std::cout<<"\nEven Client with ID: " << client->getID() << " connected successfully.";
            }
            else{
                std::cout<<"\nOdd Client with ID: " << client->getID() << " connected successfully.";
            }
        }
        client->printMenu();
        int selection = client->getMenuSelection();
        while (selection != 2){
            if (selection == 1){ //client requesting #
                    std::string ID = std::to_string(client->getID());
                    pipe_ret_t sendRet = client->sendMsg(ID.c_str(), ID.size());
                    if (!sendRet.isSuccessful()) {
                        std::cout << "\nFailed to send message: " << sendRet.message() << "\n";
                    } 
                    else {
                        std::cout << "\nRequest for a new number was sent successfuly\n";
                    }
                }
            client->printMenu();
            selection = client->getMenuSelection();
        }
        client = new TcpClient();
        client_observer_t observer;
	    observer.wantedIP = "127.0.0.1";
	    observer.incomingPacketHandler = onIncomingMsg;
	    observer.disconnectionHandler = onDisconnection;
	    client->subscribe(observer);
    }
	return 0;
}

#endif

