#include "../Execute.hpp"

static int checkJoin(std::vector<std::string> &splitted, Server *server, int channelFd)
{
    if (splitted.size() < 2 || splitted[1][0] != '#' || splitted[1].size() < 2)
        return (-1);
    if (splitted.size() == 3)
        server->getChannel(splitted[1]).setIsHasPass(true);
    if (splitted.size() > 3)
        return (-1);
    return (1);
}

void Execute::join(int &fd, Server *server, strvector splitted)
{
    int channelId = server->getChannelIndexByName(splitted[1]);
    int temp = checkJoin(splitted, server, channelId);
    if (temp == -1)
    {
        std::string cmd = "JOIN"; 
        numeric::sendNumeric(ERR_NEEDMOREPARAMS(cmd), &server->getUser(fd), server);
        return ;
    }
    if(channelId == -1)
    {
        server->createChannel(splitted[1], server->getUser(fd), splitted.size() == 3 ? splitted[2] : "");
        server->sendMessage(fd, PREFIX(server->getUser(fd)) + "JOIN " + splitted[1]);
        server->sendMessage(fd, PREFIX(server->getUser(fd)) + "MODE " + splitted[1] + " +o " + server->getUser(fd).getNickName());
        numeric::sendNumeric(RPL_NOTOPIC(server.getUser(fd).getNickname(), splitted[1]), &server->getUser(fd), server);
    }
    //if(server->getChannel(splitted[1]).getUser)
    std::cout << "join function called" << std::endl;
}