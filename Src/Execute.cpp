#include "../Includes/Execute.hpp"

Execute::Execute()
{
	commands.push_back(std::make_pair("PASS", Execute::pass));
	commands.push_back(std::make_pair("USER", Execute::user));
    commands.push_back(std::make_pair("NICK", Execute::nick));
    commands.push_back(std::make_pair("JOIN", Execute::join));
    commands.push_back(std::make_pair("CAP", Execute::cap));
    commands.push_back(std::make_pair("PRIVMSG", Execute::privmsg));
    commands.push_back(std::make_pair("TOPIC", Execute::topic));
	commands.push_back(std::make_pair("PART", Execute::part));
}

Execute::~Execute()
{
}

std::vector<std::pair<std::string, func> > Execute::getCommands() const
{
    return (this->commands);
}