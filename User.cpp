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

bool User::getCap() const { return this->cap; }

/*
* @param file descriptor for user
*/
User::User(int fd) {
	this->fd = fd;
	this->auth = false;
	this->firstLogin = true;
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

void User::setCap(bool val) { this->cap = val; }

void User::setUserAuth(Auth auth, bool val)
{
	int i;

	for( i = 0; this->auths[i].first != auth.first; i++)
		if (i == 3)
			break ;

	this->auths[i].second = val;
	checkAuths(this->getUserAuths("PASS"), this->getUserAuths("NICK"), this->getUserAuths("USER"));
}

void User::checkAuths(Auth pass, Auth nick, Auth user)
{
	if (pass.second == true && nick.second == true && user.second == true)
		this->auth = true;
}

int	User::checkPassword(std::string &inputPass, std::string password, int i)
{
	if (inputPass == password)
	{
		std::cout << "it's correct pass. User index is " << i << std::endl;
		return (1);
	}
	return (-1);
}

int User::checkNickName(std::string &nickName, std::vector<User> &temp)
{
	for(size_t i = 0; i < temp.size(); i++)
	{
		std::cout << "nicknames: " << temp[i].getNickName() << std::endl;
		if (temp[i].getNickName() == nickName)
		{
			std::cout << "nickName is already exist" << std::endl;
			return (-1);
		}
	}
	return (1);
}

void	User::addChannel(Channel newChannel)
{
	userInChannels.push_back(newChannel);
}

std::vector<Channel>	User::getChannels(void)
{
	return (this->userInChannels);
}

bool User::getUserAuth() const { return this->auth; }

Auth User::getUserAuths(std::string auth)
{
	for (int i = 0; i < 3; i++)
	{
		if (this->auths[i].first == auth)
		{
			return (this->auths[i]);
		}
	}
	return (Auth("NULL", false));
}

User::~User() {}
