#include "../../Includes/Execute.hpp"

static int checkJoin(std::vector<std::string> &splitted, Server *server)
{
	if (splitted.size() < 2 || splitted[1][0] != '#' || splitted[1].size() < 2)
		return (-1);
	if (splitted.size() == 3)
		server->getServerChannel(splitted[1])->setIsHasPass(true);
	if (splitted.size() > 3)
		return (-1);
	return (1);
}

void Execute::join(int &fd, Server *server, strvector splitted) //clientte sonradan giren kullanıcılarda önceki kullanıcılar gözükmüyor!!!
{
	User *user = server->getServerUser(fd);
	int channelId = server->getChannelIndexByName(splitted[1]);
	int temp = checkJoin(splitted, server);
	if (temp == -1)
	{
		std::string cmd = "JOIN";
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(cmd), user, server);
		return;
	}
	std::string nicknames = "";
	std::string nickname = user->getNickName();
	std::string channelName = splitted[1];
	if (channelId == -1)
	{
		server->createChannel(splitted[1], user, splitted.size() == 3 ? splitted[2] : "");
		server->sendMessage(fd, PREFIX(user) + " JOIN " + splitted[1]);
		server->sendMessage(fd, PREFIX(user) + " MODE " + splitted[1] + " +o " + user->getNickName());
		user->addChannel(server->getServerChannels()[channelId]);
		numeric::sendNumeric(RPL_NOTOPIC(user->getNickname(), splitted[1]), user, server);
		
		return;
	}
	if (server->getServerChannel(splitted[1])->getUser(user->getNickName()) == NULL)
	{
		server->sendMessage(fd, PREFIX(user) + " JOIN " + splitted[1]);

		server->getServerChannel(splitted[1])->addUser(user);
		user->addChannel(server->getServerChannel(splitted[1]));

		if (server->getServerChannel(splitted[1])->getTopic().empty())
			numeric::sendNumeric(RPL_NOTOPIC(user->getNickname(), splitted[1]), user, server);
		else
			numeric::sendNumeric(RPL_TOPIC(user->getNickname(), splitted[1], server->getServerChannel(splitted[1])->getTopic()), user, server);
	}
	std::vector<User *> users = server->getServerChannel(splitted[1])->getUsers();
	for (size_t i = 0; i < users.size(); i++)
	{
		int toSend = users[i]->getFd();
		if (toSend != fd)
			server->sendMessage(toSend, PREFIX(user) + " JOIN " + splitted[1]);
		if (i != 0)
			nicknames += " ";
		if (server->getServerChannel(splitted[1])->getOperator(users[i]) != NULL)
			nicknames += "@";
		nicknames += users[i]->getNickName();
	}
	
	numeric::sendNumeric(RPL_NAMEREPLY(nickname, channelName, nicknames), user, server);
	numeric::sendNumeric(RPL_ENDOFNAMES(nickname, channelName), user, server);
	std::cout << "join function called" << std::endl;
}