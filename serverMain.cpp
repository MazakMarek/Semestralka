#include "server.h"
#include "server.cpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "PlayingField.cpp"
#include <thread>
#include <mutex>
#pragma comment (lib, "Ws2_32.lib")

// Function to send game data to both players
void sendData(SOCKET sock,SOCKET sockHrac2 ,bool *pHraBezi, Server *pServer, PlayingField *pPlayingField, std::mutex *mut) {
    const int intervalInSeconds = 2;
    clock_t lastExecutionTime = clock();

    while(*pHraBezi){


        clock_t currentTime = clock();
        double elapsedSeconds = static_cast<double>(currentTime - lastExecutionTime) / CLOCKS_PER_SEC;

        if (elapsedSeconds >= intervalInSeconds) {
            mut->lock();
            std::cout << "Sending map." << std::endl;
            lastExecutionTime = currentTime;

            // Move and check collisions for player 1
            pPlayingField->posunHada1();
            if(!pPlayingField->getGameEndHrac1()){
                *pHraBezi = false;
                std::cout << "Player 2 crashed" << std::endl;
            }

            // Move and check collisions for player 2
            pPlayingField->posunHada2();
            if(!pPlayingField->getGameEndHrac2()){
                *pHraBezi = false;
                std::cout << "Player 1 crashed" << std::endl;
            }
            pPlayingField->makeField();
            mut->unlock();
            pServer->sendMap(sock,pPlayingField->printBoard());
            pServer->sendMap(sockHrac2,pPlayingField->printBoard());
            // vypis hracej plochy v serverovej casti
            //            std::cout << pPlayingField->printBoard();
        }
    }
}

// Function to receive input from player 1
void receiveData(SOCKET sock, bool *pHraBezi, Server *pServer, PlayingField *pPlayingField, int pNumOfPlayers, std::mutex *mut) {

    while(*pHraBezi){

        std::string responsePlayer1 = pServer->handleClient(sock);

        mut->lock();

        // Update player 1 direction if the input is valid
        if(!responsePlayer1.compare("Nothing recievied") == 0){
            pPlayingField->setSmer1(responsePlayer1[0]);
        }
        mut->unlock();
    }
}

// Function to receive input from player 2
void receiveDataHrac2(SOCKET sock, bool *pHraBezi, Server *pServer, PlayingField *pPlayingField, int pNumOfPlayers, std::mutex *mut) {
    while(*pHraBezi){

        // Receive input from player 2
        std::string responsePlayer2 = pServer->handleClient(sock);

        mut->lock();

        // Update player 2 direction if the input is valid
        if(!responsePlayer2.compare("Nothing recievied") == 0){
            pPlayingField->setSmer2(responsePlayer2[0]);
        }

        mut->unlock();
    }
}

//meain
int main(int argc, char *argv[]){
    std::mutex mut;
    Server server;
    server.startServer();
    PlayingField playingField;
    int numOfPlayers = 0;
    bool hraBezi = true;

    // Assigning sockets
    sockaddr_in clientAddress;
    int clientSize = sizeof(clientAddress);

    // Accept connection from player 1
    SOCKET clientSocketPlayer1 = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientSize);
    numOfPlayers++;
    if (clientSocketPlayer1 == INVALID_SOCKET) {
        std::cerr << "Error accepting player 1 connection" << std::endl;
        closesocket(server.getServerSocket());
        WSACleanup();
        return EXIT_FAILURE;
    } else {
        std::cout << "Connection accepted from Player 1" << std::endl;
    }

    // Accept connection from player 2
    SOCKET clientSocketPlayer2 = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientSize);
    numOfPlayers++;
    if (clientSocketPlayer2 == INVALID_SOCKET) {
        std::cerr << "Error accepting player 1 connection" << std::endl;
        closesocket(server.getServerSocket());
        WSACleanup();
        return EXIT_FAILURE;
    } else {
        std::cout << "Connection accepted from player 2" << std::endl;
    }

    // Start threads for sending and receiving data
    std::thread senderThread(sendData, clientSocketPlayer1, clientSocketPlayer2, &hraBezi, &server, &playingField, &mut);
    std::thread receiverThread(receiveData, clientSocketPlayer1, &hraBezi, &server, &playingField, numOfPlayers, &mut);
    std::thread receiverThreadHrac2(receiveDataHrac2, clientSocketPlayer2, &hraBezi, &server, &playingField, numOfPlayers, &mut);

    // Wait for threads to finish
    senderThread.join();
    receiverThread.join();
    receiverThreadHrac2.join();

    // ukoncenie Winsocketov
    WSACleanup();
    return EXIT_SUCCESS;
}