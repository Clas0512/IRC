#ifndef EXECUTE_HPP
# define EXECUTE_HPP

# include <iostream>
# include <vector>
# include <utility>
# include "../Includes/Server.hpp"
# include "../Includes/Error.hpp"
# include "../Includes/Numeric.hpp"

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
		static void join(int &fd, Server *server, strvector splitted);
		static void cap(int &fd, Server *server, strvector splitted);
		static void privmsg(int &fd, Server *server, strvector splitted);
		static void topic(int &fd, Server *server, strvector splitted);
		static void part(int &fd, Server *server, strvector splitted);
	public:
		Execute();
		~Execute();
		std::vector<std::pair<std::string, func> > getCommands() const;

};

#endif