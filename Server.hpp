#ifndef SERVER_HPP
# define SERVER_HPP

# include <fcntl.h>
# include <unistd.h>
# include <vector>
# include <netdb.h>
# include "User.hpp"
# include "Execute.hpp"
# include "Error.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>

class User;
class Execute;

using namespace Error;

struct userInfo {
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
        std::vector<Channel>	channels;
		sockaddr_in socketAddr;
		socklen_t socketAddrLen;
    public:
        Server(char **av);
        ~Server();
        void				addUser(int fd);
        std::vector<User>	&getUsers();
		User				&getUser(int fd);
        std::string			getServerName() const;
		std::string			getPassword() const;
        void				start();
		void				parseAndAdd(int fd, char *buffer);
		void				sendMessage(int fd, std::string messg);
		void				createChannel(std::string id, User &admin, std::string password);
		long				getUserIndexByFd(int fd);
		long				getChannelIndexByFd(std::string id) const;
		void				joinChannel(std::string id, User &nickname);
		int 				checkNickName(std::string &nickname);
		void				parseAndExec(int fd, std::string buffer);
};

std::vector<std::string> 	parser(std::string &buffer);
std::vector<std::string>	split(std::string str, char delimeter);

#endif