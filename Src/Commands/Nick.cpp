#include "../../Includes/Execute.hpp"

static int checkNickname(std::string &nickName, std::vector<User*> &temp)
{
	for(size_t i = 0; i < temp.size(); i++)
	{
		if (temp[i]->getNickName() == nickName)
		{
			return (-1);
		}
	}
	return (1);
}

void Execute::nick(int &fd, Server *server, strvector splitted)
{
    int temp = checkNickname(splitted[1], server->getServerUsers());
    error(temp, "Nick already in use!", FLAG_CONTINUE);
    if (temp != -1)
        server->getServerUser(fd)->setUserAuth(server->getServerUser(fd)->getUserAuths("NICK"), true);
    server->getServerUser(fd)->setNickName(splitted[1]);
    std::cout << "nick function called" << std::endl;
}