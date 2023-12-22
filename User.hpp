#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <map>
# include "Channel.hpp"

class Channel;

typedef std::pair<std::string, bool> Auth;

class User {
    private:
		int						fd;
        std::string				nickName;
        std::string				userName;
		std::vector<Channel>	userInChannels;
		bool					firstLogin;
		bool					auth;
		Auth					auths[3];
    public:
        User(int fd);
        ~User();
		int						getFd(void);
		std::string				getUserName();
		std::string				getNickName();
		bool					getUserAuth() const;
		Auth					getUserAuths(std::string auth);
		void					setSocketFd(){};
		void					setNickName(std::string &nick);
		void					setUserName(std::string &user);
		void					setUserAuth(Auth auth, bool val);
		int						checkPassword(std::string &inputPass, std::string password, int i);
		void					addChannel(Channel newChannel);
		std::vector<Channel>	getChannels(void);
		void					checkAuths(Auth pass, Auth nick, Auth user);
		int						checkNickName(std::string &nick, std::vector<User> &temp);
};


#endif