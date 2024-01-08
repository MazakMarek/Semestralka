#include "server.h"
#include "server.cpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "PlayingField.cpp"
#pragma comment (lib, "Ws2_32.lib")
#include <thread>
#include <mutex>

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

            pPlayingField->posunHada1();
            if(!pPlayingField->getGameEndHrac1()){
                *pHraBezi = false;
                std::cout << "Hrac 2 narazil" << std::endl;  // TODO aby to aj clientovy vypisalo
            }

            pPlayingField->posunHada2();
            if(!pPlayingField->getGameEndHrac2()){
                *pHraBezi = false;
                std::cout << "Hrac 1 narazil" << std::endl; // TODO aby to aj clientovy vypisalo
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

void receiveData(SOCKET sock, bool *pHraBezi, Server *pServer, PlayingField *pPlayingField, int pNumOfPlayers, std::mutex *mut) {

    while(*pHraBezi){

        std::string responsePlayer1 = pServer->handleClient(sock);

        mut->lock();

        // ak nezmenil smer tak ostane prechadzajuci smer pohybu
        if(!responsePlayer1.compare("Nothing recievied") == 0){
            pPlayingField->setSmer1(responsePlayer1[0]);
        }
        mut->unlock();
    }
}

void receiveDataHrac2(SOCKET sock, bool *pHraBezi, Server *pServer, PlayingField *pPlayingField, int pNumOfPlayers, std::mutex *mut) {

    while(*pHraBezi){

        std::string responsePlayer2 = pServer->handleClient(sock);

        mut->lock();

        // ak nezmenil smer tak ostane prechadzajuci smer pohybu
        if(!responsePlayer2.compare("Nothing recievied") == 0){
            pPlayingField->setSmer2(responsePlayer2[0]);
        }

        mut->unlock();
    }
}

int main(int argc, char *argv[]){
    std::mutex mut;
    Server server;
    server.startServer();
    PlayingField playingField;
    int numOfPlayers = 0;
    bool hraBezi = true;

    //priradenie socketov
    sockaddr_in clientAddress;
    int clientSize = sizeof(clientAddress);

    SOCKET clientSocketPlayer1 = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientSize);
    numOfPlayers++;
    if (clientSocketPlayer1 == INVALID_SOCKET) {
        std::cerr << "Chyba pri prijimani pripojenia." << std::endl;
        closesocket(server.getServerSocket());
        WSACleanup();
        return EXIT_FAILURE;
    } else {
        std::cout << "Pripojenie prijate od hraca 1." << std::endl;
    }

    SOCKET clientSocketPlayer2 = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientSize);
    numOfPlayers++;
    if (clientSocketPlayer2 == INVALID_SOCKET) {
        std::cerr << "Chyba pri prijimani pripojenia." << std::endl;
        closesocket(server.getServerSocket());
        WSACleanup();
        return EXIT_FAILURE;
    } else {
        std::cout << "Pripojenie prijate od hraca 2." << std::endl;
    }

    std::thread senderThread(sendData, clientSocketPlayer1, clientSocketPlayer2, &hraBezi, &server, &playingField, &mut);
    std::thread receiverThread(receiveData, clientSocketPlayer1, &hraBezi, &server, &playingField, numOfPlayers, &mut);
    std::thread receiverThreadHrac2(receiveDataHrac2, clientSocketPlayer2, &hraBezi, &server, &playingField, numOfPlayers, &mut);

    senderThread.join();
    receiverThread.join();
    receiverThreadHrac2.join();

    // ukoncenie Winsocketov
    WSACleanup();
    return EXIT_SUCCESS;
}