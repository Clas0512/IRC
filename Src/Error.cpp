#include "../Includes/Error.hpp"

namespace Error
{
    void error(int value, std::string func, int errorNo)
    {
        if (value < 0)
        {
            std::cout << "Error: " << func << std::endl;
            if (errorNo != FLAG_CONTINUE)
                exit(errorNo);
        }
    }
}