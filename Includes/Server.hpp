#ifndef SERVER_HPP
# define SERVER_HPP

# include <fcntl.h>
# include <unistd.h>
# include <vector>
# include <netdb.h>
# include "../Includes/User.hpp"
# include "../Includes/Execute.hpp"
# include "../Includes/Error.hpp"
# include "../Includes/Channel.hpp"
# include "../Includes/Numeric.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>

class User;
class Execute;
class Channel;

# define PREFIX(user) (":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getHostName())

using namespace Error;

class Server {
    private:
		int fd;
		int port;
		std::string				password;
        std::string				hostname;
        std::vector<User*>		serverUsers;
        std::vector<Channel*>	serverChannels;
		std::string				time;
    public:
        Server(char **av);
        ~Server();
        void					addUser(int fd);
        std::vector<User*>		&getServerUsers();
		std::vector<Channel*>	&getServerChannels();
		User*					getServerUser(int fd);
		User*					getServerUser(std::string nickname);
		Channel*				getServerChannel(std::string name);
        std::string				getServerHostName() const;
		std::string				getServerPassword() const;
        void					start();
		void					parseAndAdd(int fd, char *buffer);
		void					sendMessage(int fd, std::string messg);
		void					createChannel(std::string id, User *admin, std::string password);
		long					getUserIndexByFd(int fd);
		long					getChannelIndexByName(std::string name) const;
		long					getChannelIndexByPass(std::string id) const;
		void					joinChannel(std::string id, User *nickname);
		void					parseAndExec(int fd, std::string buffer);
		std::string				getTime(void);
};

std::vector<std::string>	 	parser(std::string &buffer);
std::vector<std::string>		split(std::string str, char delimeter);
std::string						getCurrentTime();

#endif