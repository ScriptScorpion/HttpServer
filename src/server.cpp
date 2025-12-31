#include "server.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <chrono>
#include <iostream>
#include <thread>
#define MAXPKGSIZE 4096

bool Server::Start(const int port) {
    char buff[MAXPKGSIZE] {};
    int SV_sock = socket(AF_INET, SOCK_STREAM, 0);
    int CL_sock;
    const int opt = 1;

    if (SV_sock < 0) {
        return false;
    }
    memset(&Server_addr, 0, sizeof(Server_addr));
    Server_addr.sin_family = AF_INET;
    Server_addr.sin_addr.s_addr = INADDR_ANY;
    Server_addr.sin_port = htons(port);
    if (setsockopt(SV_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { // setting that port can be used multiple time
        close(SV_sock);
        return false;
    }
    if (bind(SV_sock, (sockaddr*)&Server_addr, sizeof(Server_addr)) < 0) {
        close(SV_sock);
        return false;
    }
    if (listen(SV_sock, 2) > 0) {
        close(SV_sock);
        return false;
    }
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        CL_sock = accept(SV_sock, nullptr, nullptr);
        if (CL_sock < 0) {
            continue;
        }
        int received_bytes = recv(CL_sock, buff, sizeof(buff) - 1, 0);
        if (received_bytes > 0) {
            buff[received_bytes] = '\0';
            std::cout << buff << std::endl;
            Info_Sender(CL_sock, buff);
            memset(buff, 0, sizeof(buff));
            close(CL_sock);
        }
    }
    close(SV_sock);
    return true;
}
void Server::Info_Sender(const int CL_sock, char *buff) {
    std::string Version = "";
    for (int i = 0; i < 20; ++i) {
        if (buff[i] == '1' && buff[i + 1] == '.' && buff[i + 2] == '0') {
            Version = "1.0";
            break;
        }
        else if (buff[i] == '1' && buff[i + 1] == '.' && buff[i + 2] == '1') {
            Version = "1.1";
            break;
        }
        else if (buff[i] == '2' && buff[i + 1] == '.' && buff[i + 2] == '0') {
            Version = "2.0";
            break;
        }
    }
    std::string Response;
    Response += "HTTP/" + Version + " 200\n";
    Response += "Content-Type: text/html\n";
    Response += "Content-Length: 173\n\n";

    Response += "<!DOCTYPE html>\n";
    Response += "<html lang=\"en\">\n";
    Response += "<head>\n";
    Response += "<meta charset=\"utf-8\">\n";
    Response += "<title>local site</title>\n";
    Response += "</head>\n";
    Response += "<body>\n";
    Response += "<h1 style=\"text-align:center;\">Hello Stranger!</h1>\n";
    Response += "</body>\n";
    Response += "</html>\n\n";
    send(CL_sock, Response.c_str(), Response.size(), 0);
}
