#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <vector>
# include "Channel.hpp"

class Channel;

typedef std::pair<std::string, bool> Auth;

class User {
    private:
		int						fd;
        std::string				nickName;
        std::string				userName;
		std::vector<Channel>	userInChannels;
		bool					auth;
		Auth					auths[3];
    public:
        User(int fd);
        ~User();
        int					getFd(void);
        std::string			getUserName();
        std::string			getNickName();
        bool                getAuth();
        Auth                getUserAuths(std::string auth);
		void				setSocketFd(){};
        void                setUserAuth(Auth auth, bool val);  
		void				setNickName(std::string &nick);
		void				setUserName(std::string &user);
		int					checkPassword(std::string &inputPass, std::string password);
};


#endif