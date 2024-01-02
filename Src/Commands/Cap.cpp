#include "../../Includes/Execute.hpp"

void Execute::cap(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    if (server->getServerUser(fd)->getCap() == false)
    {
        server->sendMessage(fd, "CAP * LS :multi-prefix sasl");
        server->getServerUser(fd)->setCap(true);
    }
    else
    {
        server->sendMessage(fd, "CAP * ACK multi-prefix");
        server->getServerUser(fd)->setCap(false);
    }
    std::cout << "cap function called" << std::endl;
}