#ifndef EXECUTE_HPP
# define EXECUTE_HPP

# include <iostream>
# include <vector>
# include <utility>
# include "Server.hpp"
# include "Error.hpp"

using namespace Error;

class Server;

typedef void (*func)(int &, Server *, std::vector<std::string>);

typedef std::vector<std::string> strvector;

class Execute
{
	private:
		std::vector<std::pair<std::string, func> > commands;
		static void pass(int &fd, Server *server, strvector splitted);
		static void user(int &fd, Server *server, strvector splitted);
		static void nick(int &fd, Server *server, strvector splitted);
		static void test(int &fd, Server *server, strvector splitted);
		static void join(int &fd, Server *server, strvector splitted);
	public:
		Execute();
		~Execute();
		std::vector<std::pair<std::string, func> > getCommands() const;

};

#endif