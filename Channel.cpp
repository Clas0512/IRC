# include "Channel.hpp"

Channel::Channel(std::string _id, User _admin, std::string _password) : id(_id), password(_password)
{
	users.push_back(_admin);
}

std::string	Channel::getId(void) const
{
	return (this->id);
}

std::vector<User> Channel::getUsers() const
{
	return(this->users);
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

