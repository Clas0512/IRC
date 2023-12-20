#ifndef EXECUTE_HPP
# define EXECUTE_HPP

# include <iostream>
# include <vector>
# include <utility>
# include "Server.hpp"

class Server;

typedef void (*func)(int &, Server *, std::string, std::string);

class Execute
{
	private:
		std::vector<std::pair<std::string, func> > commands;
		static void pass(int &fd, Server *server, std::string message, std::string trimmed);
		static void user(int &fd, Server *server, std::string message, std::string trimmed);
		static void nick(int &fd, Server *server, std::string message, std::string trimmed);
	public:
		Execute();
		~Execute();
		std::vector<std::pair<std::string, func> > getCommands() const;

};

#endif
