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

int main(int argc, char *argv[]){
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



    std::cout << "Pripojenie prijate od klienta." << std::endl;

    const int intervalInSeconds = 2;
    clock_t lastExecutionTime = clock();

    while(hraBezi){
        std::string responsePlayer1 = "w"; //server.handleClient(clientSocketPlayer1);
        //std::cout << responsePlayer1 << std::endl;

        //std::string responsePlayer2 = server.handleClient(clientSocketPlayer2);
        // ci sa hraci neodpojili/ pocet aktivnych hracov
        if(responsePlayer1.compare("Client disconnected.") == 0){
            numOfPlayers--;
        }
//        if(responsePlayer2.compare("Client disconnected.") == 0){
//            numOfPlayers--;
//        }
        if(numOfPlayers == 0){
            hraBezi = false;
        }

        // ak nezmenil smer tak ostane prechadzajuci smer pohybu
        if(!responsePlayer1.compare("Nothing recievied") == 0){
            playingField.setSmer1(responsePlayer1[0]);
        }
//        if(!responsePlayer2.compare("Nothing recievied") == 0){
//            playingField.setSmer2(responsePlayer2[0]);
//        }

        clock_t currentTime = clock();
        double elapsedSeconds = static_cast<double>(currentTime - lastExecutionTime) / CLOCKS_PER_SEC;

        if (elapsedSeconds >= intervalInSeconds) {
            std::cout << "Sending map." << std::endl;
            lastExecutionTime = currentTime;
            std::cout << playingField.getSmer1();
            playingField.posunHada1();
            playingField.posunHada2();
            server.sendMap(clientSocketPlayer1,playingField.printBoard());
            //  server.sendMap(clientSocketPlayer2,playingField.printBoard());
        }

    }
//    server.handleClient(clientSocket);
    // ukoncenie Winsocketov
    WSACleanup();
    return EXIT_SUCCESS;



}