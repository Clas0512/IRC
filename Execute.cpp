#include "Execute.hpp"

Execute::Execute()
{
	commands.push_back(std::make_pair("PASS", Execute::pass));
	commands.push_back(std::make_pair("USER", Execute::user));
}

Execute::~Execute()
{
}

void	Execute::pass(int &fd, Server *server, std::string message, std::string trimmed)
{
    static_cast<void>(trimmed);
    error(server->getUser(fd).checkPassword(message, server->getPassword()), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("PASS"), true);
}

void Execute::user(int &fd, Server *server, std::string message, std::string trimmed)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(message);
    static_cast<void>(trimmed);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("USER"), true);
    std::cout << "user function" << std::endl;
}

void Execute::nick(int &fd, Server *server, std::string message, std::string trimmed)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(message);
    static_cast<void>(trimmed);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("NICK"), true);
    std::cout << "nick function" << std::endl;
}


std::vector<std::pair<std::string, func> > Execute::getCommands() const
{
    return (this->commands);
}