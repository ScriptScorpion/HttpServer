#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
class Server {
     private:
         sockaddr_in Server_addr {};
         int cl_SV;
         std::vector <int> cl_CL;
     public:
         bool Start(const int port);
         void Info_Sender(const int CL_sock, char *buff);
         ~Server() {
            close(cl_SV);
            for (int cl : cl_CL) {
                close(cl);
            }
         }
};                     
