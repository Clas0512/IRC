#include "../../Includes/Execute.hpp"

void Execute::privmsg(int &fd, Server *server, strvector splitted)
{
    if (splitted[1][0] == '#')
    {
        std::string channelName = splitted[1];
        std::string message = splitted[2];
        std::string nickname = server->getServerUser(fd)->getNickName();
        std::vector<User *> users = server->getServerChannel(channelName)->getUsers();
        for (size_t i = 0; i < users.size(); i++)
        {
            int toSend = users[i]->getFd();
            if (toSend != fd)
                server->sendMessage(toSend, PREFIX(server->getServerUser(fd)) + " PRIVMSG " + channelName + " :" + message);
        }
    }
    else
    {
        std::string nickname = splitted[1];
        std::string message = splitted[2];
        int toSend = server->getServerUser(nickname)->getFd();
        server->sendMessage(toSend, PREFIX(server->getServerUser(fd)) + " PRIVMSG " + nickname + " :" + message);
    }
    std::cout << "privmsg function called" << std::endl;
}