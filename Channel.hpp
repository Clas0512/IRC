#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include "User.hpp"

class User;

class Channel
{
	private:
		const std::string	id;
		const std::string	password;
		std::vector<User>	users;
		std::vector<User>	operators;
	public:
		Channel(std::string _id, User _admin, std::string _password);
		std::string	getId(void) const;
		bool	checkPassword(std::string password) const;
		std::vector<User> getUsers() const;
		void	addUser(User &newUser);
};

#endif