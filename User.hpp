#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <map>
# include "Channel.hpp"
# include "Execute.hpp"
# include "Numeric.hpp"

class Channel;

typedef std::pair<std::string, bool> Auth;

class User {
    private:
		int						fd;
		int 					mode;
        std::string				nickName;
        std::string				userName;
		std::string				realName;
		std::string				hostName;
		std::vector<Channel>	userInChannels;
		bool					firstLogin;
		bool					cap;
		bool					auth;
		Auth					auths[3];
    public:
        User(int fd);
        ~User();
		int						getFd(void) const;
		std::string				getUserName() const;
		std::string				getNickName() const;
		std::string				getRealName() const;
		std::string				getHostName() const;
		bool					getFirstLogin() const;
		bool					getCap() const;
		bool					getUserAuth() const;
		Auth					getUserAuths(std::string auth);
		void					setSocketFd(){};
		void					setCap(bool val);
		void					setMode(int mode);
		void					setNickName(std::string &nick);
		void					setUserName(std::string &user);
		void					setHostName(std::string &host);
		void					setRealName(std::string &real);
		void					setFirstLogin(bool val);
		void					setUserAuth(Auth auth, bool val);
		int						checkPassword(std::string &inputPass, std::string password, int i);
		int						checkNickName(std::string &nick, std::vector<User> &temp);
		int						checkUser(std::vector<std::string> &splitted, Server *server);
		void					addChannel(Channel newChannel);
		std::vector<Channel>	getChannels(void);
		void					checkAuths(Auth pass, Auth nick, Auth user);
};


#endif