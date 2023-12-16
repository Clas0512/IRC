#ifndef SERVER_HPP
# define SERVER_HPP

# include <fcntl.h>
# include <unistd.h>
# include <vector>
# include <netdb.h>
# include "User.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>

class User;

struct userInfo{
	std::string userName;
	std::string nickName;
};

class Server {
    private:
		int fd;
		int port;
		std::string			password;
        std::string			serverName;
        std::vector<User>	users;
		struct sockaddr_in socketAddr;
		socklen_t socketAddrLen;
    public:
        Server(char **av);
        ~Server();
        void				addUser(int fd);
        std::vector<User>	&getUsers();
        std::string			getServerName() const;
        void				start();
		void				error(int value, std::string func, int errorNo);
		void				parseAndAdd(int _fd, char *buffer);
		void				sendMessage(int fd, std::string messg);
};

#endif