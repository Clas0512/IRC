#include "../../Includes/Execute.hpp"

static int	checkPassword(std::string &inputPass, std::string password, int i)
{
	if (inputPass == password)
	{
		std::cout << "it's correct pass. User index is " << i << std::endl;
		return (1);
	}
	return (-1);
}

void	Execute::pass(int &fd, Server *server, strvector splitted)
{
    if (server->getServerUser(fd)->getUserAuths("PASS").second == true && server->getServerUser(fd)->getUserAuth() == true)
    {
        numeric::sendNumeric(ERR_ALREADYREGISTRED, server->getServerUser(fd), server);
        return;
    }
    error(checkPassword(splitted[1], server->getServerPassword(), server->getServerUsers().size()), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
    server->getServerUser(fd)->setUserAuth(server->getServerUser(fd)->getUserAuths("PASS"), true);
    std::cout << "pass func called" << std::endl;
}