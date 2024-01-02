#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include "../Includes/User.hpp"

class User;

class Channel
{
	private:
		const std::string	name;
		const std::string	password;
		std::string			topic;
		std::string			owner;
		bool				isHasPass;
		std::vector<User *>	users;
		std::vector<User *>	operators;
	public:
		Channel(std::string _id, User *_admin, std::string _password);
		bool				checkPassword(std::string password) const;
		void				addUser(User *newUser);
		bool				getIsHasPass(void) const;
		void				setIsHasPass(bool val);
		void				setTopic(std::string topic);
		std::vector<User*>	getOperators() const;
		std::vector<User*> 	getUsers() const;
		User*				getUser(std::string nick) const;
		User*				getOperator(User *user);
		std::string			getName(void) const;
		std::string			getTopic(void) const;
		std::string			getPassword(void) const;
};

#endif