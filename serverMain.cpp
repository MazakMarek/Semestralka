#include "server.h"
#include "PlayingField.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

void updateGameState(PlayingField& game) {
    while (game.isGameEnd()) {
        // Acquire lock before updating the game state
        game.lockGameState();

        // Update the game state
        game.posunHada1();
        game.posunHada2();

        // Release lock after updating the game state
        game.unlockGameState();

        // Sleep for a short duration to control the speed of the game
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int argc, char* argv[]) {
    Server server;
    server.startServer();
    PlayingField playingField;
    int numOfPlayers = 0;
    bool hraBezi = true;
    // priradenie socketov
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

    std::cout << "Pripojenie prijate od klienta." << std::endl;

    const int intervalInSeconds = 2;
    clock_t lastExecutionTime = clock();

    while (hraBezi) {
        std::string responsePlayer1 = "w"; //server.handleClient(clientSocketPlayer1);
        // std::cout << responsePlayer1 << std::endl;

        // std::string responsePlayer2 = server.handleClient(clientSocketPlayer2);
        // ci sa hraci neodpojili/ pocet aktivnych hracov
        if (responsePlayer1.compare("Client disconnected.") == 0) {
            numOfPlayers--;
        }

        if (numOfPlayers == 0) {
            hraBezi = false;
        }

        // ak nezmenil smer tak ostane prechadzajuci smer pohybu
        if (!responsePlayer1.compare("Nothing recievied") == 0) {
            playingField.setSmer1(responsePlayer1[0]);
        }

        clock_t currentTime = clock();
        double elapsedSeconds = static_cast<double>(currentTime - lastExecutionTime) / CLOCKS_PER_SEC;

        if (elapsedSeconds >= intervalInSeconds) {
            std::cout << "Sending map." << std::endl;
            lastExecutionTime = currentTime;
            std::cout << playingField.getSmer1();
            updateGameState(playingField);
            server.sendMap(clientSocketPlayer1, playingField.printBoard());
            //  server.sendMap(clientSocketPlayer2,playingField.printBoard());
        }
    }

    // ukoncenie Winsocketov
    closesocket(clientSocketPlayer1);
    WSACleanup();
    return EXIT_SUCCESS;
}
