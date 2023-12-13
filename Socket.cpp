#include "Socket.hpp"

Socket::Socket()
{
    socklen_t socketAddrLen = sizeof(socketAddr);
    char buffer[1024];
    int opt = 1;

    if ((this->socketFd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0)
    {
        std::cout << "Socket creation cannot completed!" << std::endl;
        exit(101);
    }

    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "Socket option setting cannot completed!" << std::endl;
        exit(101);
    }

    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port = htons(4242);
    socketAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketFd, (struct sockaddr*)&socketAddr, socketAddrLen))
    {
        std::cout << "Socket binding cannot completed!" << std::endl;
        exit(101);
    }

    if (listen(socketFd, 3) < 0)
    {
        std::cout << "Listen does not work!" << std::endl;
        exit(102);
    }

    if ((recvSocket = accept(socketFd, (struct sockaddr*)&socketAddr, &socketAddrLen)) < 0)
    {
        std::cout << "Couldn't accept!" << std::endl;
        exit(103);
    }

    if (recv(recvSocket, buffer, 1024, MSG_PEEK) < 0)
    {
        std::cout << "Couldn't receive" << std::endl;
        exit(104);
    }

    std::cout << buffer << std::endl;
}

Socket::~Socket()
{
    
}