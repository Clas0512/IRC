#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <fcntl.h>
# include <netdb.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include "User.hpp"
# include "Error.hpp"
# include "Execute.hpp"

using namespace Error;
using namespace std;

class User;

class Server {
    private:
		int fd;
		int port;
		string			    password;
        string			    serverName;
        vector<User>	    users;
		sockaddr_in			socketAddr;
		socklen_t			socketAddrLen;
    public:
        Server(char **av);
        ~Server();
        void				addUser(int fd);
        std::vector<User>	&getUsers();
		User				&getUser(int fd);
        string	    		getServerName() const;
		string  			getPassword() const;
        void				start();
		void				parseAndAdd(int fd, char *buffer);
		void				sendMessage(int fd, string messg);
		void				parse(int fd, string buffer);
        vector<string>		parseBuffer(const string &buffer, char delimiter);
        void				compare(vector<string> message, string trimmed, int fdIndex);
};

#endif