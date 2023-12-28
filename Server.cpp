#include "Server.hpp"
#include <sstream>
#include <stdlib.h>

Server::Server(char **av)
{
	int opt = 1;
	char hostName[1024];
	gethostname(hostName, 1024);
	this->hostname = hostName;
	port = std::stoi(av[1]);
	this->password = av[2];
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
	error(fd, "Socket creation cannot completed!", 101);
	sockaddr_in socketAddr;
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = INADDR_ANY;
	socketAddr.sin_port = htons(port);

	error(bind(fd, reinterpret_cast<sockaddr*>(&socketAddr), sizeof(socketAddr)), "Socket binding cannot completed!", 101);
	this->time = getTime();
	error(listen(this->fd, 20), "Listen does not work!", 102);
	error(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "Socket option setting cannot completed!", 101);
	error(fcntl(fd, F_SETFL, O_NONBLOCK), "in fcntl function!", 107);
}

Server::~Server() {}

void Server::addUser(int fd)
{
	users.push_back(User(fd));
	std::cout << users[0].getUserName() << std::endl;
}

/*
* This is start the all events
*/
void Server::start()
{
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
				sockaddr_in clientAddr;
				int clientFd = accept(this->fd, reinterpret_cast<sockaddr*>(&clientAddr), reinterpret_cast<socklen_t*>(&clientAddr));
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
				if (pollFds[i].revents == 0)
					continue;
				if (pollFds[i].revents != POLLIN) // BURAYA KULLANICI SİLMEKTE EKLENECEK ÖNEMLİ !!!
				{
					std::cout << "A user disconnected!" << std::endl;
					close(pollFds[i].fd);
					pollFds.erase(pollFds.begin() + i);
					i--;
				}
				else if (pollFds[i].revents & POLLIN)
				{
					error(recv(pollFds[i].fd, buffer, 1024, MSG_EOF), "Couldn't recieve!", 108);
					//parseAndAdd(pollFds[i].fd, buffer);
					std::cout << "girdi: " << buffer << std::endl;
					std::string buff = buffer;
					parseAndExec(pollFds[i].fd, buff);
				}
			}
		}
	}
}

void	Server::parseAndExec(int fd, std::string buffer)
{
	Execute exec;
	std::vector<std::string> buff = parser(buffer);
	for(size_t i = 0; i < buff.size(); i++)
	{
		std::vector<std::string> splitted = split(buff[0], ' ');
		 for(size_t i = 0; i < splitted.size(); i++)
		 	std::cout << "splitted: "  << splitted[i] << std::endl;
		for(size_t j = 0; j < exec.getCommands().size(); j++)
		{
			if (exec.getCommands()[j].first == splitted[0])
			{
				exec.getCommands()[j].second(fd, this, splitted);
				if(this->getUser(fd).getUserAuth() == true && this->getUser(fd).getFirstLogin() == true)
				{
					this->getUser(fd).setFirstLogin(false);
					std::string nickname = this->getUser(fd).getNickName();
					std::string username = this->getUser(fd).getUserName();
					std::string hostname = this->getHostName();
					std::string time = getTime();
					numeric::sendNumeric(RPL_WELCOME(nickname, username, hostname), &(this->getUser(fd)), this);
					numeric::sendNumeric(RPL_YOURHOST(nickname, hostname), &(this->getUser(fd)), this);
					numeric::sendNumeric(RPL_CREATED(nickname, time), &(this->getUser(fd)), this);
				}
			}
		}
	}
}

void	Server::parseAndAdd(int fd, char *buffer)
{
	bool isJoin = false;
	bool isFirst = false;
	long i = getUserIndexByFd(fd);
	if (i == -1)
	{
		perror("Filedescriptor is not valid. Olamaaz programi durdurun !!!?!");
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
	int ic = 0;
	while (s != e)
	{
		if (*s == "PASS" && (s + 1) != e)
		{
			s++;
			//error(this->users[i].checkPassword(*s, this->password, i), "Wrong Password! Please enter correct password.", FLAG_CONTINUE);
			sign++;
		}
		else if (*s == "NICK" && (s + 1) != e)
		{
			s++;
			std::string nick(*s);
			// if(checkNickName(nick) == -1)
			// {
			// 	std::string messg = ": 433 : " + this->users[i].getUserName() + " " + nick + " :Nickname is already in use\r\n";
			// 	sendMessage(fd, messg);
			// }
			//else
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
			int channelId = getChannelIndexByName(*s);
			std::cout << "channel id: " << channelId << std::endl;
			if (!((*s).size() > 2 && (*s)[0] == '#'))
			{
				std::cout << "Channel id is not valid!" << std::endl;
				return ;
			}
			if (channelId == -1)
			{
				createChannel(*s, users[i], *(s + 1));
				std::cout << this->users[i].getChannels().size() << "\n";
				isJoin = true;
				isFirst = true;
			}
			else
			{
				if (channels[channelId].checkPassword(*(s + 1)) == false)
				{
					std::cout << "Invalid password!" << std::endl;
					break ;
				}
				joinChannel(*s, users[i]);
				users[i].addChannel(channels[channelId]);
				for (size_t j = 0; j < users.size(); j++)
				{
					User temp = channels[channelId].getUsers()[j];
					users[j].getChannels()[users[j].getChannels().size() - 1].addUser(temp);
					// users[j].getChannels()[users[j].getChannels().size() - 1].addUser(temp);
				}
				
				isJoin = true;
			}
		}
		
		if (*s == "CLIST" && (s + 1) != e)
		{
			int id = getChannelIndexByName(*(s + 1));
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

		if (*s == "PRIVMSG" && (s + 1) != e && (s + 2) != e)
		{
			s++;
			int ChannelIndex = getChannelIndexByPass(*s);
			for (size_t j = 0; j < users[i].getChannels()[users[i].getChannels().size() - 1].getUsers().size(); j++)
			{
				std::cout << "firdim" << "\n";
				// server->getUser(fd)) + " PRIVMSG " + toWho + " " + msg);
				std::string mssgPrv = ":" + this->users[i].getNickName() + "!" + this->users[i].getUserName() + "@" + "localhost" + " PRIVMSG " + channels[ChannelIndex].getName() + " :" + *(s + 1) + "\r\n";
				sendMessage(users[i].getChannels()[users[i].getChannels().size() - 1].getUsers()[j].getFd(), mssgPrv);
			}
			
		}

		std::cout << "-----------------------------------" << "\n";
		for (size_t j = 0; j < users.size(); j++)
		{
			std::cout << users[j].getNickName() << "\n" << "\n";
			for (size_t h = 0; h < users[j].getChannels().size(); h++)
			{
				for (size_t k = 0; k < users[j].getChannels()[h].getUsers().size(); k++)
				{
					std::cout << users[j].getChannels()[h].getUsers()[k].getNickName() << std::endl;
				}
			}
			std::cout << "\n";
		}
		

		// ------------------------------------------------------
		s++;
	}
	if (sign == 3)
	{
		// macroyla olusturulacak
		// std::string messg = ": 001 : " + this->users[i].getUserName() + " Welcome to the Internet Relay Network " + this->users[i].getNickName() + "!" + this->users[i].getUserName() + "@" + this->getServerName() + "\r\n"; 
		// sendMessage(fd, messg);
		numeric::sendNumeric(RPL_WELCOME(users[i].getNickName(), users[i].getUserName(), getHostName()), &(this->users[i]), this);
		numeric::sendNumeric(RPL_YOURHOST(users[i].getNickName(), getHostName()), &(this->users[i]), this);
		numeric::sendNumeric(RPL_CREATED(users[i].getNickName(), this->getTime()), &(this->users[i]), this);
	}
	if (isJoin == true)
	{
		// std::string messg = ": 331 :" + this->users[i].getUserName() + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " " + ":topic" + "\r\n";
		// std::string message = ": 331 : " + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + ":No topic is set" + "\r\n";
		// sendMessage(fd, message);
		// ( "=" / "*" / "@" ) <channel>
        //        :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
		
		ic++;
		
		std::string mssgJoin = ":" + this->users[i].getNickName() + "!" + this->users[i].getUserName() + "@" + "localhost" + " JOIN " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getPassword() + "\r\n";
		sendMessage(fd, mssgJoin);
		if (isFirst == true)
		{
			// [!] BUFFER: :nickim!userim@localhost JOIN #5187 3131
			// [!] BUFFER: :nickim!userim@localhost MODE #5187 3131 +o nickim
			// [!] BUFFER: :k1m01s02.42istanbul.com.tr 331 nickim #5187 3131 :No topic is set
			std::string mssgJoin2 = ":" + this->users[i].getNickName() + "!" + this->users[i].getUserName() + "@" + "localhost" + " MODE " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getPassword() + " +o " + this->users[i].getNickName() + "\r\n";
			sendMessage(fd, mssgJoin2);
			std::string mssg = ":" + this->getHostName() + " 331 " + this->getUsers()[i].getNickName() + " " + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getPassword() + " :" + "No topic is set" + "\r\n";
			sendMessage(fd, mssg);
		}
		else
		{
			// if (ic == 2)
			// sendMessage(fd, mssgJoin);
			std::string mssgJoin2 = ":" + this->getHostName() + " 353 " + this->users[i].getNickName() + " = " + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getPassword() + " :@"; //+ this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getUsers()[i].getNickName() + "\r\n";
			for (long j = 0; j < this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getUsers().size(); j++)
			{
				sendMessage(this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getUsers()[j].getFd(), mssgJoin);
				if (j == 0)
					mssgJoin2 = mssgJoin2 + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getUsers()[j].getNickName();
				else
					mssgJoin2 = mssgJoin2 + " " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getUsers()[j].getNickName();
			}
			mssgJoin2 = mssgJoin2 + "\r\n";
			
			sendMessage(fd, mssgJoin2);
			std::string mssg = ":" + this->getHostName() + " 331 " + this->getUsers()[i].getNickName() + " " + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " " + this->users[i].getChannels()[this->users[i].getChannels().size() - 1].getPassword() + " :" + "No topic is set" + "\r\n";
			sendMessage(fd, mssg);
			// [!] BUFFER: :k1m01s02.42istanbul.com.tr 366 newbie|2 #ad 123 :End of /NAMES list
			// :k1m01s02.42istanbul.com.tr 366 nickim #4542 3131 :End of /NAMES list
			std::string mssg1 = ":" + this->getHostName() + " 366 " + this->getUsers()[i].getNickName() + " " + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " " + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getPassword() + " :End of /NAMES list" + "\r\n";
			sendMessage(fd, mssg1);
		}
		//messg = ": 353 : " + this->users[i].getUserName() + " = " + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + " : " + this->users[i].getNickName() + "\r\n";
		//sendMessage(fd, messg);
		//messg = ": 366 : " + this->users[i].getUserName() + (*this).users[i].getChannels()[this->users[i].getChannels().size() - 1].getName() + "\r\n";
		//sendMessage(fd, messg);
	}
}

void Server::sendMessage(int fd, std::string messg)
{
	std::cout << "send func:" << messg << " to " << fd << std::endl;
	messg += "\r\n";
	error(send(fd, messg.c_str(), messg.size() + 1, 0), "Couldn't send!", 31);
}

long	Server::getUserIndexByFd(int fd)
{
	for (size_t i = 0; i < users.size(); i++)
		if (users[i].getFd() == fd)
			return (i);
	return (-1);
}

long	Server::getChannelIndexByName(std::string name) const
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == name)
			return (i);
	}
	return (-1);
}

long	Server::getChannelIndexByPass(std::string id) const
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getPassword() == id)
			return (i);
	}
	return (-1);
}

std::vector<User> &Server::getUsers() { return (users); }

User &Server::getUser(int fd)
{
	for (size_t i = 0; i < users.size(); i++)
		if (users[i].getFd() == fd)
			return (users[i]);
	return (users[0]); // burayi duzelt
}

std::string Server::getHostName() const { return (hostname); }

std::string Server::getPassword() const { return (password); }

void	Server::createChannel(std::string id, User &admin, std::string password)
{
	Channel newChannel(id, admin, password);
	this->channels.push_back(newChannel);
	admin.addChannel(newChannel);
}

void	Server::joinChannel(std::string id, User &nickname)
{
	int idf = getChannelIndexByName(id);
	std::vector<User> temp = this->channels[idf].getUsers();
	for (size_t i = 0; i < temp.size(); i++)
		if (temp[i].getFd() == nickname.getFd())
		{
			std::cout << "User is already exist!" << std::endl;
			return ;
		}
	this->channels[idf].addUser(nickname);
}

std::string getCurrentTime()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	std::time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
	return std::string(buffer);
}

std::string Server::getTime(void)
{
	return (time);
}