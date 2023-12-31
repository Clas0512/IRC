#include "../../Includes/Execute.hpp"

static int checkUser(std::vector<std::string> &splitted, Server *server, User *user)
{
	if (splitted.size() < 5 )
	{
		std::string command = "USER";
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(command), user, server);
		return (-1);
	}
	for(size_t i = 1; i < splitted.size(); i++)
	{
		if(splitted[i].empty())
		{
			std::cout << "space basti" << std::endl;
			return -1;
		}
		user->setUserName(splitted[1]);
		try
		{
			user->setMode(stoi(splitted[2]));
		}
		catch(const std::exception& e)
		{
			return (-1);
		}
		user->setHostName(splitted[3]);
		std::string realname;
		for(size_t j = 4; j < splitted.size(); j++)
			realname += splitted[j] + " ";
		user->setRealName(realname);
	}
	return (1);
}

void Execute::user(int &fd, Server *server, strvector splitted)
{    
    if (server->getServerUser(fd)->getUserAuths("USER").second == true && server->getServerUser(fd)->getUserAuth() == true)
    {
        numeric::sendNumeric(ERR_ALREADYREGISTRED, server->getServerUser(fd), server);
        return;
    }
    int tmp = checkUser(splitted, server, server->getServerUser(fd));
    if (tmp == -1)
	{
		std::string command = "USER";
		numeric::sendNumeric(ERR_NEEDMOREPARAMS(command), server->getServerUser(fd), server);
		return;
	}
    server->getServerUser(fd)->setUserAuth(server->getServerUser(fd)->getUserAuths("USER"), true);
    std::cout << "user function called" << std::endl;
}