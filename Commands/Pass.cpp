#include "../Execute.hpp"

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
    if (server->getUser(fd).getUserAuths("PASS").second == true && server->getUser(fd).getUserAuth() == true)
    {
        numeric::sendNumeric(ERR_ALREADYREGISTRED, &server->getUser(fd), server);
        return;
    }
    error(checkPassword(splitted[1], server->getPassword(), server->getUsers().size()), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("PASS"), true);
    std::cout << "pass func called" << std::endl;
}