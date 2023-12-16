#ifndef USER_HPP
# define USER_HPP

# include <iostream>

class User {
    private:
		int					fd;
        std::string			nickName;
        std::string			userName;
    public:
        User(int fd);
        ~User();
        int					getFd(void);
        std::string			getUserName();
        std::string			getNickName();
		void				setSocketFd(){};
		void				setNickName(std::string &nick);
		void				setUserName(std::string &user);
};


#endif