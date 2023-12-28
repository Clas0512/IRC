#include "../Execute.hpp"

static int checkJoin(std::vector<std::string> &splitted)
{
    if (splitted.size() < 2 || splitted[1][0] != '#' || splitted[1].size() < 2)
        return (-1);
    
    return (1);
}

void Execute::join(int &fd, Server *server, strvector splitted)
{
    int temp = checkJoin(splitted);
    std::cout << "join function called" << std::endl;
}