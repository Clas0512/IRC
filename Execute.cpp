#include "Execute.hpp"

Execute::Execute()
{
	int a = 1;
	Server *b;
	std::string c;
	commands.push_back(std::make_pair("PASS", Execute::pass));
	commands.push_back(std::make_pair("USER", Execute::user));
}