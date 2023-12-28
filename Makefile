NAME = ircserv
CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
COMMANDS = ./Commands

SRCS = $(wildcard *.cpp $(COMMANDS)/*.cpp) #Channel.cpp main.cpp parser.cpp Server.cpp User.cpp 
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)


$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all