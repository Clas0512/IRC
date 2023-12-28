#include "User.hpp"
#include "Server.hpp"


// @param fd file descriptor for user
User::User(int fd) {
	this->fd = fd;
	this->auth = false;
	this->firstLogin = true;
	this->auths[0] = Auth("PASS", false);
	this->auths[1] = Auth("USER", false);
	this->auths[2] = Auth("NICK", false);
}

//SETTERS

void				User::setNickName(std::string &nick) { this->nickName = nick; }
void				User::setUserName(std::string &user) { this->userName = user; }
void				User::setHostName(std::string &host) { this->hostName = host; }
void				User::setRealName(std::string &real) { this->realName = real; }
void				User::setMode(int mode) { this->mode = mode; }
void 				User::setCap(bool val) { this->cap = val; }
void 				User::setFirstLogin(bool val) { this->firstLogin = val; }

void 				User::setUserAuth(Auth auth, bool val)
{
	int i;
	for( i = 0; this->auths[i].first != auth.first; i++)
		if (i == 3)
			break ;
	this->auths[i].second = val;
	checkAuths(this->getUserAuths("PASS"), this->getUserAuths("NICK"), this->getUserAuths("USER"));
}

//GETTERS

int User::getFd(void) const { return (fd); }
std::string User::getUserName(void) const { return (userName); }
std::string User::getNickName(void) const { return (nickName); }
std::string User::getRealName(void) const { return (realName); }
std::string User::getHostName(void) const { return (hostName); }
bool User::getFirstLogin(void) const { return (firstLogin); }
bool User::getCap() const { return this->cap; }

//COMMAND CHECKERS

void User::checkAuths(Auth pass, Auth nick, Auth user)
{
	if (pass.second == true && nick.second == true && user.second == true)
		this->auth = true;
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
