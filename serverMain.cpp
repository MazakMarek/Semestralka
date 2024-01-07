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

void sendData(SOCKET sock, bool pHraBezi, Server *pServer, PlayingField *pPlayingField, std::mutex *mut) {
    bool hraBezi = pHraBezi;
    const int intervalInSeconds = 2;
    clock_t lastExecutionTime = clock();

    while(hraBezi){


        clock_t currentTime = clock();
        double elapsedSeconds = static_cast<double>(currentTime - lastExecutionTime) / CLOCKS_PER_SEC;

        if (elapsedSeconds >= intervalInSeconds) {
            mut->lock();
            std::cout << "Sending map." << std::endl;
            lastExecutionTime = currentTime;
           // std::cout << pPlayingField.getSmer1();

            pPlayingField->posunHada1();
            if(!pPlayingField->getGameEndHrac1()){
                hraBezi = false;
                std::cout << "Hrac 2 narazil" << std::endl;  // TODO aby to aj clientovy vypisalo
            }

            pPlayingField->posunHada2();
            if(!pPlayingField->getGameEndHrac2()){
                hraBezi = false;
                std::cout << "Hrac 1 narazil" << std::endl; // TODO aby to aj clientovy vypisalo
            }
            pPlayingField->makeField();
            mut->unlock();
            pServer->sendMap(sock,pPlayingField->printBoard());
            std::cout << pPlayingField->printBoard();
//              server.sendMap(clientSocketPlayer2,playingField.printBoard());

        }

    }


}

void receiveData(SOCKET sock, bool pHraBezi, Server *pServer, PlayingField *pPlayingField, int pNumOfPlayers, std::mutex *mut) {

    while(pHraBezi){


        std::string responsePlayer1 = pServer->handleClient(sock);
        std::cout << responsePlayer1 << std::endl;
        std::cout << pPlayingField->getSmer1() << std::endl;
        std::cout << ('w' == responsePlayer1[0]) << std::endl;

        //std::string responsePlayer2 = server.handleClient(clientSocketPlayer2);
        mut->lock();
        // ci sa hraci neodpojili/ pocet aktivnych hracov
        if(responsePlayer1.compare("Client disconnected.") == 0){
            pNumOfPlayers--;
        }
//        if(responsePlayer2.compare("Client disconnected.") == 0){
//            numOfPlayers--;
//        }
        if(pNumOfPlayers == 0){
            pHraBezi = false;
        }

        // ak nezmenil smer tak ostane prechadzajuci smer pohybu
        if(!responsePlayer1.compare("Nothing recievied") == 0){
            pPlayingField->setSmer1(responsePlayer1[0]);
        }
//        if(!responsePlayer2.compare("Nothing recievied") == 0){
//            pPlayingField.setSmer2(responsePlayer2[0]);
//        }
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
    }


//    SOCKET clientSocketPlayer2 = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientSize);
//    numOfPlayers++;
//    if (clientSocketPlayer2 == INVALID_SOCKET) {
//        std::cerr << "Chyba pri prijimani pripojenia." << std::endl;
//        closesocket(server.getServerSocket());
//        WSACleanup();
//        return EXIT_FAILURE;
//    }

    std::thread senderThread(sendData, clientSocketPlayer1, hraBezi, &server, &playingField, &mut);
    std::thread receiverThread(receiveData, clientSocketPlayer1, hraBezi, &server, &playingField, numOfPlayers, &mut);

    std::cout << "Pripojenie prijate od klienta." << std::endl;



//    while(hraBezi){
//        std::string responsePlayer1 = server.handleClient(clientSocketPlayer1);
//        //std::cout << responsePlayer1 << std::endl;
//
//        //std::string responsePlayer2 = server.handleClient(clientSocketPlayer2);
//        // ci sa hraci neodpojili/ pocet aktivnych hracov
//        if(responsePlayer1.compare("Client disconnected.") == 0){
//            numOfPlayers--;
//        }
////        if(responsePlayer2.compare("Client disconnected.") == 0){
////            numOfPlayers--;
////        }
//        if(numOfPlayers == 0){
//            hraBezi = false;
//        }
//
//        // ak nezmenil smer tak ostane prechadzajuci smer pohybu
//        if(!responsePlayer1.compare("Nothing recievied") == 0){
//            playingField.setSmer1(responsePlayer1[0]);
//        }
////        if(!responsePlayer2.compare("Nothing recievied") == 0){
////            playingField.setSmer2(responsePlayer2[0]);
////        }
//
//        clock_t currentTime = clock();
//        double elapsedSeconds = static_cast<double>(currentTime - lastExecutionTime) / CLOCKS_PER_SEC;
//
//        if (elapsedSeconds >= intervalInSeconds) {
//            std::cout << "Sending map." << std::endl;
//            lastExecutionTime = currentTime;
//            std::cout << playingField.getSmer1();
//            playingField.posunHada1();
//            playingField.posunHada2();
//            playingField.makeField();
//            server.sendMap(clientSocketPlayer1,playingField.printBoard());
//            //  server.sendMap(clientSocketPlayer2,playingField.printBoard());
//        }
//
//    }
//    server.handleClient(clientSocket);

    senderThread.join();
    receiverThread.join();
    // ukoncenie Winsocketov
    WSACleanup();
    return EXIT_SUCCESS;



}