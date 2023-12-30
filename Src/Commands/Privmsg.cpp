#include "../../Includes/Execute.hpp"

void Execute::privmsg(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    std::cout << "privmsg function called" << std::endl;
}