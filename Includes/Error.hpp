#ifndef ERROR_HPP
# define ERROR_HPP

# include <iostream>

# define FLAG_CONTINUE 1

namespace Error
{
    void error(int value, std::string func, int errorNo);
}

#endif