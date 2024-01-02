//
// Created by pc on 02/01/2024.
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef UNTITLED1_SERVER_H
#define UNTITLED1_SERVER_H
class Server{
private:
    int serverSocket;
    int cisloPortu;

public:
    void startServer();

};


#endif //UNTITLED1_SERVER_H
