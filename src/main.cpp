#include "server.hpp"
#include <iostream>
int main() {
    Server instance;
    int port;
    std::cout << "Enter port: ";
    std::cin >> port;
    if (!std::cin || port <= 1024 || port >= 65535) {
        std::cerr << "Error: Invalid input" << std::endl;
        return 2;
    }
    if (!instance.Start(port)) {
        return 1;
    }
    return 0;
}
