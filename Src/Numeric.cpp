#include "../Includes/Numeric.hpp"

namespace numeric
{
	Numeric makeNumeric(std::string code, std::string message)
	{
		Numeric numeric;

		numeric.first = code;
		numeric.second = message;
		return (numeric);
	}
	void sendNumeric(Numeric reply, User *user, Server *server)
	{
		int fd = user->getFd();
		server->sendMessage(fd, ":" + server->getHostName() + " " + reply.first + " " + user->getNickName() + " " + reply.second);
	}
}