#include "../../Includes/Execute.hpp"

void Execute::privmsg(int &fd, Server *server, strvector splitted)
{
	if(splitted[2][0] != ':')
	{
		;
	}
	std::string msg = server->getCurrentMessage();
	msg = msg.substr(msg.find(":") + 1);
	std::cout << "cikti " << msg << std::endl;
    if (splitted[1][0] == '#')
    {
        std::string channelName = splitted[1];
        std::string nickname = server->getServerUser(fd)->getNickName();
        std::vector<User *> users = server->getServerChannel(channelName)->getUsers();
        for (size_t i = 0; i < users.size(); i++)
        {
            int toSend = users[i]->getFd();
            if (toSend != fd)
                server->sendMessage(toSend, PREFIX(server->getServerUser(fd)) + " PRIVMSG " + channelName + " :" + msg);
        }
    }
    else
    {
        std::string nickname = splitted[1];
        int toSend = server->getServerUser(nickname)->getFd();
        server->sendMessage(toSend, PREFIX(server->getServerUser(fd)) + " PRIVMSG " + nickname + " :" + msg);
    }
    std::cout << "privmsg function called" << std::endl;
}