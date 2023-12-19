NAME = ircserv
CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)


$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all