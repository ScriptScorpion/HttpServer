#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
class Server {
     private:
         sockaddr_in Server_addr {};
     public:
         bool Start(const int port);
         void Info_Sender(const int CL_sock, char *buff);
};                     
