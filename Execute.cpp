#include "Execute.hpp"

Execute::Execute()
{
	commands.push_back(std::make_pair("PASS", Execute::pass));
	commands.push_back(std::make_pair("USER", Execute::user));
    commands.push_back(std::make_pair("NICK", Execute::nick));
    commands.push_back(std::make_pair("JOIN", Execute::join));
    commands.push_back(std::make_pair("CAP", Execute::cap));
    commands.push_back(std::make_pair("TEST", Execute::test)); // temp
}

Execute::~Execute()
{
}

//PASS command
void	Execute::pass(int &fd, Server *server, strvector splitted)
{
    if (server->getUser(fd).getUserAuths("PASS").second == true && server->getUser(fd).getUserAuth() == true)
    {
        numeric::sendNumeric(ERR_ALREADYREGISTRED, &server->getUser(fd), server);
        return;
    }
    error(server->getUser(fd).checkPassword(splitted[1], server->getPassword(), server->getUsers().size()), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("PASS"), true);
    std::cout << "pass func called" << std::endl;
}

//USER command
void Execute::user(int &fd, Server *server, strvector splitted)
{    
    if (server->getUser(fd).getUserAuths("USER").second == true && server->getUser(fd).getUserAuth() == true)
    {
        numeric::sendNumeric(ERR_ALREADYREGISTRED, &server->getUser(fd), server);
        return;
    }
    int tmp = server->getUser(fd).checkUser(splitted, server);
    error(tmp, "You should enter \"USER <username> <mode> <hostname> <realname>\"", FLAG_CONTINUE);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("USER"), true);
    std::cout << "user function called" << std::endl;
}

//NICK command
void Execute::nick(int &fd, Server *server, strvector splitted)
{
    int temp = server->getUser(fd).checkNickName(splitted[1], server->getUsers());
    error(temp, "Nick already in use!", FLAG_CONTINUE);
    if (temp != -1)
        server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("NICK"), true);
    server->getUser(fd).setNickName(splitted[1]);
    std::cout << "nick function called" << std::endl;
}

//JOIN command
void Execute::join(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    server->getUser(fd).setUserAuth(server->getUser(fd).getUserAuths("JOIN"), true);
    std::cout << "join function called" << std::endl;
}

//CAP command
void Execute::cap(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    if (server->getUser(fd).getCap() == false)
    {
        server->sendMessage(fd, "CAP * LS :multi-prefix sasl");
        server->getUser(fd).setCap(true);
    }
    else
    {
        server->sendMessage(fd, "CAP * ACK multi-prefix");
        server->getUser(fd).setCap(false);
        sleep(1);
    }
    std::cout << "cap function called" << std::endl;
}

void Execute::test(int &fd, Server *server, strvector splitted)
{
    static_cast<void>(fd);
    static_cast<void>(server);
    static_cast<void>(splitted);
    if (server->getUser(fd).getUserAuth() == false)
    {
        Error::error(-1, "You should enter NICK, USER and PASS arguments", FLAG_CONTINUE);
        return;
    }
    std::cout << "test function called" << std::endl;
}


std::vector<std::pair<std::string, func> > Execute::getCommands() const
{
    return (this->commands);
}