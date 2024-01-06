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
    Server();
    void startServer();
    int getServerSocket();
    std::string handleClient(SOCKET clientSocket);
    void sendMap(SOCKET clientSocket, std::string map);
};


#endif //UNTITLED1_SERVER_H
