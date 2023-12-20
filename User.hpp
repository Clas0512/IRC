#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <vector>
# include <sstream>
# include "Channel.hpp"

class Channel;

typedef std::pair<std::string, bool> Auth;

class User {
    private:
		int						fd;
        std::string				nickName;
        std::string				userName;
        std::string				realName;
		std::vector<Channel>	userInChannels;
		bool					auth;
		Auth					auths[3];
    public:
        User(int fd);
        ~User();
        int					getFd(void);
        std::string			getUserName();
        std::string			getNickName();
        std::string			getRealName();
        bool                getAuth();
        Auth                getUserAuths(std::string auth);
		void				setSocketFd(){};
        void                setUserAuth(Auth auth, bool val);  
		void				setNickName(std::string &nick);
		void				setUserName(std::string &user);
        void				setRealName(std::string &user);
		int					checkPassword(std::string &inputPass, std::string password);
        int                 checkUser(std::string &message, std::string trimmed);
        int                 findWordCount(const std::string &str);
};


#endif