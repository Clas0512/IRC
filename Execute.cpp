#include "Execute.hpp"

Execute::Execute()
{
	commands.push_back(std::make_pair("PASS", Execute::pass));
	commands.push_back(std::make_pair("USER", Execute::user));
    commands.push_back(std::make_pair("NICK", Execute::nick));
    commands.push_back(std::make_pair("TEST", Execute::test));
}

Execute::~Execute()
{
}

void	Execute::pass(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(server);
    static_cast<void>(splitted);
    static_cast<void>(fd);
    std::cout << "buraya girdi" << std::endl;
    error(server->getUser(fd).checkPassword(splitted[1], server->getPassword(), server->getUsers().size()), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("PASS"), true);
    std::cout << "pass func called" << std::endl;
}

void Execute::user(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    // error(server->getUser(fd).checkUser(splitted, trimmed), "Wrong User! Please enter correct user.", FLAG_CONTINUE);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("USER"), true);
    std::cout << "user function called" << std::endl;
}

void Execute::nick(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("NICK"), true);
    std::cout << "nick function called" << std::endl;
}

void Execute::test(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    std::cout << "pass: " << server->getUser(fd).getUserAuths("PASS").second << std::endl;
    std::cout << "nick: " << server->getUser(fd).getUserAuths("NICK").second << std::endl;
    std::cout << "user: " << server->getUser(fd).getUserAuths("USER").second << std::endl;
    if (server->getUser(fd).getUserAuth() == false)
    {
        Error::error(-1, "You should enter NICK, USER and PASS arguments", FLAG_CONTINUE);
        return;
    }
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("NICK"), true);
    std::cout << "test function called" << std::endl;
}


std::vector<std::pair<std::string, func> > Execute::getCommands() const
{
    return (this->commands);
}