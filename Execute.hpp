#ifndef EXECUTE_HPP
# define EXECUTE_HPP

# include <iostream>
# include <vector>
# include "Server.hpp"
# include <utility>

class
{
	private:
		std::vector<std::pair<std::string, std::function<void(int &, Server *, std::string)>>> commands;
		static void pass(int &fd, Server *server, std::string func);
		static void user(int &fd, Server *server, std::string func);
	public:
		Execute();

};

#endif