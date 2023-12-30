NAME = ircserv
CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I include

SRCDIR = Src
COMMANDS = $(SRCDIR)/Commands
OBJDIR = Obj


SRCS = $(wildcard $(SRCDIR)/*.cpp $(COMMANDS)/*.cpp) #Channel.cpp main.cpp parser.cpp Server.cpp User.cpp 
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)


$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all