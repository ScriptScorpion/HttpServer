#include "server.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#define MAXPKGSIZE 4096

bool Server::Start(const int &port) {
    char buff[MAXPKGSIZE] {};
    memset(buff, '\0', sizeof(buff));
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
    if (listen(SV_sock, 1) < 0) {
        close(SV_sock);
        return false;
    }

    std::cout << "\nStarted listening on this address http://127.0.0.1:" << port << "\n\n";
    
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
            Parse(CL_sock, buff);
            memset(buff, 0, sizeof(buff));
            close(CL_sock);
        }
    }
    close(SV_sock);
    return true;
}
void Server::Parse(const int &CL_sock, char *buff) {
    std::ifstream File("/path/to/file.html", std::ios::ate);
    if (!File) {
        std::cerr << "File you provided don't exists" << std::endl;
        close(CL_sock);
        close(SV_sock);
        exit(1);
    }
    std::string Response;
    Response += "HTTP/1.1 200 OK\r\n";
    Response += "Content-Type: text/html\r\n";
    Response += "Content-Length: " + std::to_string(File.tellg()) + "\r\n\r\n";
    File.seekg(0);
    char ch;
    while (File.get(ch)) {
        Response += ch;
    }
    File.close();
    send(CL_sock, Response.c_str(), Response.size(), 0);
}
