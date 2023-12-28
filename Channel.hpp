#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include "User.hpp"

class User;

class Channel
{
	private:
		const std::string	name;
		const std::string	password;
		std::vector<User>	users;
		std::vector<User>	operators;
	public:
		Channel(std::string _id, User _admin, std::string _password);
		std::string	getName(void) const;
		bool	checkPassword(std::string password) const;
		std::vector<User> getUsers() const;
		void	addUser(User newUser);
		std::string	getPassword(void) const;
};

#endif