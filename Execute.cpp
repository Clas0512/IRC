#include "Execute.hpp"

Execute::Execute()
{
	commands.push_back(std::make_pair("PASS", Execute::pass));
	commands.push_back(std::make_pair("USER", Execute::user));
    commands.push_back(std::make_pair("NICK", Execute::nick));
}

Execute::~Execute()
{
}

void	Execute::pass(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(server);
    static_cast<void>(splitted);
    static_cast<void>(fd);
    error(server->getUser(fd).checkPassword(splitted, server->getPassword()), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
    //server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("PASS"), true);
    std::cout << "pass func called" << std::endl;
}

void Execute::user(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    // error(server->getUser(fd).checkUser(splitted, trimmed), "Wrong User! Please enter correct user.", FLAG_CONTINUE);
    // server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("USER"), true);
    std::cout << "user function called" << std::endl;
}

void Execute::nick(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    // server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("NICK"), true);
    std::cout << "nick function called" << std::endl;
}


std::vector<std::pair<std::string, func> > Execute::getCommands() const
{
    return (this->commands);
}