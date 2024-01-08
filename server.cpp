#include <chrono>
#include "server.h"
using namespace std::chrono;

Server::Server() {}

// Function to start the server and initialize Winsock
void Server::startServer(){
    // inicializacia Win sock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error while init Winsock." << std::endl;
        return;
    }

    // Create a socket
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket == -1) {
        std::cerr << "Error while reating a socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Set up addres adn port
    cisloPortu = 8008; //Vyber portu
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(cisloPortu);

    //Bind socket to address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket to address" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    //Listen for incoming connection
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on the socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    std::cout << "Server is running and listening on port number " << cisloPortu << std::endl;
}

// Function to get the server socket
int Server::getServerSocket() {
    return this->serverSocket;
}

// Function to handle communication with a client
std::string Server::handleClient(SOCKET clientSocket) {

    char buffer[1024];
    int bytesRead;

        // Receive data from the client
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                return "Client disconnected."; // TODO prerobit to ku vsetkym vypisom

            } else {
                return "Nothing recievied";
            }
        }


        // Print received message
        buffer[bytesRead] = '\0';
        return std::string(buffer);
}

// Function to send a game map to a client
void Server::sendMap(SOCKET clientSocket, std::string map) {
    if (send(clientSocket, map.c_str(), map.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Chyba pri odosielaní odpovede klientovi." << std::endl;
    }
}
