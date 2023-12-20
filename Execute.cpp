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

void	Execute::pass(int &fd, Server *server, std::string message, std::string trimmed)
{
    static_cast<void>(trimmed);
    std::cout << "normal pass: " << server->getPassword() << std::endl;
    std::cout << "gelen pass: " << message << std::endl;
    error(server->getUser(fd).checkPassword(message, server->getPassword()), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("PASS"), true);
}

void Execute::user(int &fd, Server *server, std::string message, std::string trimmed)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(message);
    error(server->getUser(fd).checkUser(message, trimmed), "Wrong User! Please enter correct user.", FLAG_CONTINUE);
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