#include "../../Includes/Execute.hpp"

void Execute::part(int &fd, Server *server, strvector splitted)
{
	bool isThird = false;
	if (splitted.size() == 2 || splitted.size() == 3)
	{
		if (!splitted[2].empty())
			isThird = true;
		if (isThird == true)
		{
			std::vector<std::string> tmp(3);
			tmp.push_back("PRIVMSG");
			tmp.push_back(splitted[1]);
			tmp.push_back(splitted[2]);
			Execute::privmsg(fd, server, tmp);
		}
		User *tmpUser = server->getServerUser(fd);
		Channel *tmpChannel = server->getServerChannel(splitted[1]);
		for (size_t i = 0; i < tmpUser->getChannels().size(); i++)
		{
			if (tmpUser->getChannels()[i]->getName() == splitted[1])
			{
				std::cout << "segmennt" << std::endl;
				tmpUser->getChannels().erase(tmpUser->getChannels().begin() + i);

			}
		}

		for (size_t i = 0; i < tmpChannel->getUsers().size(); i++)
		{
			if (tmpChannel->getUsers()[i]->getFd() == fd)
				tmpChannel->getUsers().erase(tmpChannel->getUsers().begin() + i);
		}

		for (size_t i = 0; i < tmpChannel->getUsers().size(); i++)
		{
			if (isThird == true)
				server->sendMessage(fd, PREFIX(tmpUser) + " PART " + splitted[1] + " :" + splitted[2]);
			else
				server->sendMessage(fd, PREFIX(tmpUser) + " PART " + splitted[1]);
		}
	}
}