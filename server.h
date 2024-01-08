#ifndef UNTITLED1_SERVER_H
#define UNTITLED1_SERVER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

class Server{
private:
    int serverSocket;
    int cisloPortu;

public:
    // Constructor for the Server class
    Server();

    // Method to start the server and initialize the server socket
    void startServer();

    // Getter method to retrieve the server socket
    int getServerSocket();

    // Method to handle communication with a client
    // Returns the received message from the client
    std::string handleClient(SOCKET clientSocket);

    // Method to send a map to a client
    void sendMap(SOCKET clientSocket, std::string map);
};


#endif //UNTITLED1_SERVER_H
