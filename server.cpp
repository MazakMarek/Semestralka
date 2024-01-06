//
// Created by pc on 02/01/2024.
//
#include <chrono>
#include "server.h"
using namespace std::chrono;

Server::Server() {}

void Server::startServer(){
    // inicializaciu Win socketov
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Chyba pri inicializácii Winsock." << std::endl;
        return;
    }

    // vytvorenie socketu
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket == -1) {
        std::cerr << "Chyba pri vytváraní soketu." << std::endl;
        exit(EXIT_FAILURE);
    }

    //Nastavenie adresy a portu
    cisloPortu = 8008; //Vyber portu
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(cisloPortu);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Chyba pri viazani socketu na adresu." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Chyba pri pocuvani na sockete." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    std::cout << "Server bezi a pocuva na porte " << cisloPortu << std::endl;

}

int Server::getServerSocket() {
    return this->serverSocket;
}

std::string Server::handleClient(SOCKET clientSocket) {


    // Set the desired waiting time


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
        buffer[bytesRead] = '\0';  // Null-terminate the received data
        return std::string(buffer);

//        // Send a response back to the client
//        const char *responseMessage = "Hello from the server!";
//        if (send(clientSocket, responseMessage, strlen(responseMessage), 0) == SOCKET_ERROR) {
//            std::cerr << "Chyba pri odosielaní odpovede klientovi." << std::endl;
//        }


//     Clean up
//    closesocket(clientSocket);
}

void Server::sendMap(SOCKET clientSocket, std::string map) {
    if (send(clientSocket, map.c_str(), map.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Chyba pri odosielaní odpovede klientovi." << std::endl;
    }

}
