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

std::string User::getRealName(void) {
	return (realName);
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

void				User::setRealName(std::string &user)
{
	this->realName = user;
}

bool                User::getAuth()
{
	return (this->auth);
}

Auth User::getUserAuths(std::string auth)
{
	for (int i = 0; i < 3; i++)
		if (this->auths[i].first == auth)
			return (this->auths[i]);
	return (Auth("NULL", false));
}

void User::setUserAuth(Auth auth, bool val)
{
	auth.second = val;
}

int	User::checkPassword(std::string &inputPass, std::string password)
{
	if (inputPass == password)
	{
		this->setUserAuth(this->getUserAuths("PASS"), true);
		if (this->getUserAuths("PASS").second == true && this->getUserAuths("USER").second == true && this->getUserAuths("NICK").second == true)
			this->auth = true;
		std::cout << "it's correct pass!" << std::endl;
		return (1);
	}
	return (-1);
}

int User::checkUser(std::string &message, std::string trimmed)
{
	static_cast<void>(message);
	std::string buff = trimmed.substr(trimmed.find(" ") + 1);
	std::cout << "buff: " << buff << std::endl;
	int a = findWordCount(buff);
	std::cout << a << std::endl;
	int i = -1;
	if (a == 4)
	{
		for(int j = 0; j < 4; j++)
		{
			i = trimmed.find(" ", i + 1);
			std::cout << "i: " << i << std::endl;
			if (j == 1)
			{
				std::string user = buff.substr(0, buff.find(" "));
				std::cout << "user: " << user << std::endl;
				this->setUserName(user);
			}
			else if (j == 3)
			{
				std::string real = buff.substr(buff.find(i) + 1);
				std::cout << "nick: " << real << std::endl;
				this->setRealName(real);
			}

		} 
	}


	return (1);
}

int User::findWordCount(const std::string& str) {
    std::istringstream iss(str);
    std::string word;
    int wordCount = 0;

    while (iss >> word) {
        wordCount++;
    }

    return wordCount;
}

User::~User() {}
