#include "Server.hpp"
#include <sstream>
#include <stdlib.h>

Server::Server(char **av)
{
	int opt = 1;
	char hostName[1024];
	gethostname(hostName, 1024);
	serverName = hostName;
	port = std::stoi(av[1]);
	this->socketAddrLen = sizeof(socketAddr);
	this->password = av[2];

	fd = socket(AF_INET, SOCK_STREAM, 0);
	error(fd, "Socket creation cannot completed!", 101);
	error(setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)), "Socket option setting cannot completed!", 101);

	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = INADDR_ANY;
	socketAddr.sin_port = htons(port);

	error(bind(fd, (struct sockaddr *)&socketAddr, socketAddrLen), "Socket binding cannot completed!", 101);
}

Server::~Server() {}

void Server::addUser(int fd)
{
	users.push_back(User(fd));
}

void Server::start()
{
	// struct pollfd myPoll[2];
	// int denemeFd = poll(myPoll, );

// ------------------------------------------------------------------
	error(listen(this->fd, 3), "Listen does not work!", 102);

	std::vector<pollfd> pollFds(1);
	pollFds[0].fd = this->fd;
	pollFds[0].events = POLLIN;

	while (true)
	{
		int errorPoll = poll(&pollFds[0], pollFds.size(), POLLERR);
		error(errorPoll, "POLLERROR", 105);
		if (errorPoll == 0)
			continue;
		else
		{
			if (pollFds[0].revents & POLLIN) // ----------------**********************----------**********-*-*-*-*-*-**
			{
				int clientFd = accept(this->fd, reinterpret_cast<sockaddr*>(&this->socketAddr), reinterpret_cast<socklen_t*>(&this->socketAddrLen));
				error(fcntl(clientFd, F_SETFL, O_NONBLOCK), "in fcntl function!", 107);
				pollfd temp;
				temp.fd = clientFd;
				temp.events = POLLIN;
				pollFds.push_back(temp);
				addUser(clientFd);
				std::cout << "A new user connected!" << std::endl;
			}
			for (size_t i = 1; i < pollFds.size(); i++)
			{
				char buffer[1024];
				memset(buffer, 0, 1024);
				if (pollFds[i].revents & POLLIN)
				{
					error(recv(pollFds[i].fd, buffer, 1024, MSG_EOF), "Couldn't recieve!", 108);
					//parseAndAdd(pollFds[i].fd, buffer);
					parse(pollFds[i].fd, buffer);
				}
			}
		}
	}
}


// void	Server::parseAndAdd(int fd, char *buffer)
// {
// 	size_t i;
// 	std::string buff(buffer);
// 	for (i = 0; i < this->users.size(); i++)
// 		if (this->users[i].getFd() == fd)
// 			break ;
	
// 	std::istringstream ss(buff);
// 	std::vector<std::string> words;
// 	std::string word;
//     while (ss >> word) {
//         words.push_back(word);
//     }
// 	std::vector<std::string>::iterator s = words.begin();
// 	std::vector<std::string>::iterator e = words.end();
// 	int sign = 0;
// 	Execute exec;
// 	while (s != e)
// 	{
		
// 		// if (s == exec.commands[i].first)
// 		// {
// 		// 	exec.commands[i].second(fd, this, *s);
// 		// 	sign++;
// 		// }
// 		if (*s == "PASS" && (s + 1) != e)
// 		{
// 			s++;
// 			error(this->users[i].checkPassword(*s, this->password), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
// 			sign++;
// 		}
// 		else if (*s == "NICK" && (s + 1) != e)
// 		{
// 			s++;
// 			std::string nick(*s);
// 			this->users[i].setNickName(nick);
// 			sign++;
// 		}

// 		else if (*s == "USER" && (s + 1) != e)
// 		{
// 			s++;
// 			std::string user(*s);
// 			this->users[i].setUserName(user);
// 			sign++;
// 		}
// 		s++;
// 	}
// 	if (sign == 3)
// 	{
// 		// macroyla olusturulacak
// 		std::string messg = ": 001 : " + this->users[i].getUserName() + " Welcome to the Internet Relay Network " + this->users[i].getNickName() + "!" + this->users[i].getUserName() + "@" + this->getServerName() + "\r\n"; 
// 		sendMessage(fd, messg);
// 	}
// }

void Server::sendMessage(int fd, std::string messg)
{
	error(send(fd, messg.c_str(), messg.size() + 1, 0), "Couldn't send!", 31);
}

std::vector<User> &Server::getUsers() { return (users); }

std::string Server::getServerName() const { return (serverName); }

std::string Server::getPassword() const { return (password); }

User &Server::getUser(int fd)
{
	for (size_t i = 0; i < users.size(); i++)
		if (users[i].getFd() == fd)
			return (users[i]);
	return (users[0]); // dummy return
}


//UTILS

void	Server::parse(int fd, std::string buffer)
{
	size_t fdIndex;
	for (fdIndex = 0; fdIndex < this->users.size(); fdIndex++)
		if (this->users[fdIndex].getFd() == fd)
			break ;
	bool flag = false;
	string trimmed = buffer.substr(0, buffer.find("\r\n"));
	std::vector<string> message = parseBuffer(trimmed, ' ');
	for(size_t i = 0; i < message.size(); i++)
		std::cout << message[i] << std::endl;
	if (this->getUser(fd).getAuth() == true)
	{
		cout << "buraya girdi2" << endl;
		compare(message, trimmed, fdIndex);
	}
	else
	{
		cout << "buraya girdi3" << endl;
		compare(message, trimmed, fdIndex);
	}
	if (this->getUser(fd).getAuth() == true && flag == false)
	{
		std::string messg = ": 001 : " + this->getUser(fd).getUserName() + " Welcome to the Internet Relay Network " + this->getUser(fd).getNickName() + "!" + this->getUser(fd).getUserName() + "@" + this->getServerName() + "\r\n"; 
 		sendMessage(fd, messg);
		flag = true;
	}
}

void Server::compare(std::vector<string> message, string trimmed, int fdIndex)
{
	Execute exec;
	for (size_t i = 0; i < exec.getCommands().size(); i++)
	{
		if (exec.getCommands()[i].first == message[0])
		{
			exec.getCommands()[i].second(fdIndex, this, message[1], trimmed);
			return ;
		}
	}
}

vector<string> Server::parseBuffer(const string &buffer, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0, end = 0;

    while ((end = buffer.find(delimiter, start)) != std::string::npos) {
        token = buffer.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
    }

    token = buffer.substr(start);
    tokens.push_back(token);

    return tokens;
}