#include "../Includes/Server.hpp"

using namespace std;

int main(int ac, char **av)
{
	if (ac == 3)
	{
		Server *mainServer = new Server((av));
		while (true)
		{
			mainServer->start();
		}
	}
	else
		cout << "Error: Invalid arguments.\nYou should enter 3 agruments." << endl;
    return (0);
}