#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include "User.hpp"

class User;

class Channel
{
	private:
		//User				owner;
		std::string			name;
		std::vector<User>	users;
		std::vector<User>	operators;
	public:
};

#endif