#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

int main(int ac, char **av)
{
    int socketFd;
    int recvSocket;
    struct sockaddr_in socketAddr;
    socklen_t socketAddrLen = sizeof(socketAddr);
    char buffer[4096];
    int opt = 1;
    
        if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            std::cout << "Socket creation cannot completed!" << std::endl;
            exit(101);
        }

        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
        {
            std::cout << "Socket option setting cannot completed!" << std::endl;
            exit(101);
        }

        socketAddr.sin_family = AF_INET;
        socketAddr.sin_addr.s_addr = INADDR_ANY;
        socketAddr.sin_port = htons(4242);

        if (bind(socketFd, (struct sockaddr *)&socketAddr, socketAddrLen))
        {
            std::cout << "Socket binding cannot completed!" << std::endl;
            exit(101);
        }

        if (listen(socketFd, 3) < 0)
        {
            std::cout << "Listen does not work!" << std::endl;
            exit(102);
        }

        if ((recvSocket = accept(socketFd, (struct sockaddr *)&socketAddr, &socketAddrLen)) < 0)
        {
            std::cout << "Couldn't accept!" << std::endl;
            exit(103);
        }

    while (1)
    {
        if (recv(recvSocket, buffer, 4097, MSG_PEEK) < 0)
        {
            std::cout << "Couldn't receive" << std::endl;
            exit(104);
        }
         std::cout << "buffer:" << buffer << std::endl;

        // printf("This is a New Connection,The socket file descriptor is %d and the IP is : %s on Port : %d\\n", recvSocket , inet_ntoa(socketAddr.sin_addr) , ntohs(socketAddr.sin_port));

std::string servername("k1m01s02.42istanbul.com.tr");
std::string kullaniciAdi("kivircikali");
std::string kullaniciHost("localhost");
std::string rplWelcomeMessage = ":" + servername + " " + "001" + kullaniciAdi +
                                    "Welcome to the Internet Relay Network " +
                                    kullaniciAdi + "!" + kullaniciAdi + "@" +
                                    kullaniciHost;
        if (send(recvSocket, rplWelcomeMessage.c_str(), rplWelcomeMessage.size(), 0) < 0)
        {
            std::cout << "Couldn't send" << std::endl;
            exit(104);
        }
    // "Welcome to the Internet Relay Network <nick>!<user>@<host>"
    }
    return (0);
}