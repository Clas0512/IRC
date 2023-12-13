#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>

class Socket{
    private:
        int socketFd;
        int recvSocket;
        struct sockaddr_in socketAddr;
    public:
        Socket();
        ~Socket();
};


#endif