#include "User.hpp"
#include "Server.hpp"

int User::getFd(void) {
    return (fd);
}

std::string User::getUserName(void) {
    return (userName);
}

std::string User::getNickName(void) {
    return (nickName);
}

User::User(int fd) {
	this->fd = fd;
	this->auth = false;
	this->auths[0] = Auth("PASS", false);
	this->auths[1] = Auth("USER", false);
	this->auths[2] = Auth("NICK", false);
}

void				User::setNickName(std::string &nick)
{
	this->nickName = nick;
}

void				User::setUserName(std::string &user)
{
	this->userName = user;
}

int	User::checkPassword(std::string &inputPass, std::string password, int i)
{
	if (inputPass == password)
	{
		this->auth = true;
		std::cout << "it's correct pass. User index is " << i << std::endl;
		return (1);
	}
	return (-1);
}

User::~User() {}
