//
// Created by pc on 02/01/2024.
//
#include "server.h"


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
        std::cerr << "Chyba pri viazaní socketu na adresu." << std::endl;
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
    std::cout << "Server beží a pocuva na porte " << cisloPortu << std::endl;
}



int main(int argc, char *argv[]){





    //priradenie socketov
    sockaddr_in clientAddress;
    int clientSize = sizeof(clientAddress);

    SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Chyba pri prijímaní pripojenia." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    std::cout << "Pripojenie prijaté od klienta." << std::endl;
    // ukoncenie Winsocketov
    WSACleanup();
    return EXIT_SUCCESS;
}