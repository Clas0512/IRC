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
	std::cout << users[0].getUserName() << std::endl;
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
					std::cout << buffer;
					parseAndAdd(pollFds[i].fd, buffer);
				}
			}
		}
	}
}

void	Server::parseAndAdd(int fd, char *buffer)
{
	bool isJoin = false;
	long i = getUserIndexByFd(fd);
	if (i == -1)
	{
		perror("Filedescriptor is not valid. Olamaaz programı durdurun !!!?!");
		exit(1111);
	}
	std::string buff(buffer);
	std::istringstream ss(buff);
	std::vector<std::string> words;
	std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
	std::vector<std::string>::iterator s = words.begin();
	std::vector<std::string>::iterator e = words.end();
	int sign = 0;
	while (s != e)
	{
		if (*s == "PASS" && (s + 1) != e)
		{
			s++;
			error(this->users[i].checkPassword(*s, this->password, i), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
			sign++;
		}
		else if (*s == "NICK" && (s + 1) != e)
		{
			s++;
			std::string nick(*s);
			this->users[i].setNickName(nick);
			sign++;
		}
		else if (*s == "USER" && (s + 1) != e)
		{
			s++;
			std::string user(*s);
			this->users[i].setUserName(user);
			sign++;
		}

		// GECİCİ KOMUTLAR


		if (*s == "USERLIST")
		{
			std::vector<User> temp = getUsers();
			std::vector<User>::iterator b = temp.begin();
			std::vector<User>::iterator e = temp.end();
			while (b != e)
			{
				std::string tempMessg((*b).getUserName());
				std::cout << (*b).getUserName() << std::endl;
				sendMessage(fd, tempMessg);
				b++;
			}
		}
		
		if (*s == "JOIN" && (s + 1) != e && (s + 2) != e)
		{
			s++;
			int channelId = getChannelIndexByFd(*s);
			if (!((*s).size() > 2 && (*s)[0] == '#'))
			{
				std::cout << "Channel id is not valid!" << std::endl;
				return ;
			}
			if (channelId == -1)
				createChannel(*s, users[getUserIndexByFd(fd)], *(s + 1));
			else
			{
				if (channels[channelId].checkPassword(*(s + 1)) == false)
				{
					std::cout << "Invalid password!" << std::endl;
					break ;
				}
				joinChannel(*s, users[getUserIndexByFd(fd)]);
				isJoin = true;
			}
		}
		
		if (*s == "CLIST" && (s + 1) != e)
		{
			int id = getChannelIndexByFd(*(s + 1));
			error(id, "Channel id is not valid!", FLAG_CONTINUE);
			if (id == -1)
				break ;
			std::vector<User> temp = channels[id].getUsers();
			std::vector<User>::iterator b = temp.begin();
			std::vector<User>::iterator e = temp.end();
			while (b != e)
			{
				std::string tempMessg((*b).getUserName());
				std::cout << (*b).getUserName() << std::endl;
				sendMessage(fd, tempMessg);
				b++;
			}
		}

		// ------------------------------------------------------
		s++;
	}
	if (sign == 3)
	{
		// macroyla olusturulacak
		std::string messg = ": 001 : " + this->users[i].getUserName() + " Welcome to the Internet Relay Network " + this->users[i].getNickName() + "!" + this->users[i].getUserName() + "@" + this->getServerName() + "\r\n"; 
		sendMessage(fd, messg);
	}
	if (isJoin == true)
	{
		std::string messg = ": 332 : " + this->users[i].getUserName() + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getId() + " " + ":topic" + "\r\n";
		sendMessage(fd, messg);
	}
}

void Server::sendMessage(int fd, std::string messg)
{
	error(send(fd, messg.c_str(), messg.size() + 1, 0), "Couldn't send!", 31);
}

void Server::error(int value, std::string func, int errorNo)
{
	if (value < 0)
	{
		std::cout << "Error: " << func << std::endl;
		if (errorNo != FLAG_CONTINUE)
			exit(errorNo);
	}
}

long	Server::getUserIndexByFd(int fd)
{
	for (size_t i = 0; i < users.size(); i++)
		if (users[i].getFd() == fd)
			return (i);
	return (-1);
}

long	Server::getChannelIndexByFd(std::string id) const
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getId() == id)
			return (i);
	}
	return (-1);
}

std::vector<User> &Server::getUsers() { return (users); }

std::string Server::getServerName() const { return (serverName); }

void	Server::createChannel(std::string id, User &admin, std::string password)
{
	this->channels.push_back(Channel(id, admin, password));
}

void	Server::joinChannel(std::string id, User &nickname)
{
	int idf = getChannelIndexByFd(id);
	std::vector<User> temp = this->channels[idf].getUsers();
	for (size_t i = 0; i < temp.size(); i++)
		if (temp[i].getFd() == nickname.getFd())
		{
			std::cout << "User is already exist!" << std::endl;
			return ;
		}
	this->channels[idf].addUser(nickname);
}