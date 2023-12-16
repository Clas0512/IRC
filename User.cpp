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
}

void				User::setNickName(std::string &nick)
{
	this->nickName = nick;
}

void				User::setUserName(std::string &user)
{
	this->userName = user;
}

User::~User() {}
