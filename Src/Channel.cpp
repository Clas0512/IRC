# include "../Includes/Channel.hpp"

Channel::Channel(std::string _id, User _admin, std::string _password) : name(_id), password(_password)
{
	this->isHasPass = false;
	users.push_back(_admin);
}

std::string	Channel::getName(void) const
{
	return (this->name);
}

std::vector<User> Channel::getUsers() const
{
	return(this->users);
}

std::vector<User> Channel::getOperators() const
{
	return(this->operators);
}

void	Channel::addUser(User newUser)
{
	users.push_back(newUser);
	// newUser.addChannel(*this);
}

bool	Channel::checkPassword(std::string password) const
{
	if (password == this->password)
		return (true);
	return (false);
}

std::string	Channel::getPassword(void) const
{
	return (this->password);
}

std::string	Channel::getTopic(void) const
{
	return (this->topic);
}

User	Channel::getUser(std::string nick) const
{
	for (std::vector<User>::const_iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->getNickName() == nick)
			return (*it);
	}
	return (User(-1));
}

User Channel::getOperator(User user)
{
	for(size_t i = 0; i < this->operators.size(); i++)
	{
		if (this->operators[i].getNickName() == user.getNickName())
			return (this->operators[i]);
	}
	return (User(-1));
}

bool	Channel::getIsHasPass(void) const
{
	return (this->isHasPass);
}

void	Channel::setIsHasPass(bool val)
{
	this->isHasPass = val;
}

void	Channel::setTopic(std::string topic)
{
	this->topic = topic;
}