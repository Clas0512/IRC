#include "../../Includes/Execute.hpp"

static int checkTopic(strvector splitted, Server *server, int &fd)
{
    std::string cmd = "TOPIC";
    if (splitted.size() < 3 || splitted[1][0] != '#' || splitted[2][0] != ':' || splitted[1].size() < 2)
    {
        numeric::sendNumeric(ERR_NEEDMOREPARAMS(cmd), &server->getUser(fd), server);
        return (-1);
    }
    if (server->getChannelIndexByName(splitted[1]) == -1)
    {
        numeric::sendNumeric(ERR_NOSUCHCHANNEL(splitted[1]), &server->getUser(fd), server);
        return (-1);
    }
    if (server->getChannel(splitted[1]).getUser(server->getUser(fd).getNickName()).getFd() == -1)
    {
        numeric::sendNumeric(ERR_NOTONCHANNEL(splitted[1]), &server->getUser(fd), server);
        return (-1);
    }
    return (1);
}

static std::string createTopicMessage(strvector splitted)
{
    std::string topic = "";
    for (size_t i = 2; i < splitted.size(); i++)
    {
        if (i != 2)
            topic += " ";
        topic += splitted[i];
    }
    return (topic);
}

void Execute::topic(int &fd, Server *server, strvector splitted)
{
    int temp = checkTopic(splitted, server, fd);
    if (temp == -1)
    {
        Error::error(temp, "checkTopic", FLAG_CONTINUE);
        return ;
    }
    std::string topic = createTopicMessage(splitted);
    Channel *channel = &server->getChannel(splitted[1]);
    channel->setTopic(topic);
    server->sendMessage(fd, PREFIX(server->getUser(fd)) + " TOPIC " + splitted[1] + " :" + topic);
    std::vector<User> users = channel->getUsers();
    for (size_t i = 0; i < users.size(); i++)
    {
        int toSend = users[i].getFd();
        if (toSend != fd)
            server->sendMessage(toSend, PREFIX(server->getUser(fd)) + "TOPIC " + splitted[1] + " :" + topic);
    }
    if (topic.empty())
        numeric::sendNumeric(RPL_NOTOPIC(server->getUser(fd).getNickName(), splitted[1]), &server->getUser(fd), server);
    else
        numeric::sendNumeric(RPL_TOPIC(server->getUser(fd).getNickName(), splitted[1], topic), &server->getUser(fd), server);
    std::cout << "topic function called" << std::endl;
}