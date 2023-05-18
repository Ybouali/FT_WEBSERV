
NAME = webserv

CC = clang++

FLAGS = --std=c++98 -Wall -Wextra -Werror -fsanitize=address 

SRCS = src/webserv.cpp \
	   src/utils.cpp \
	   src/configuration/Location.cpp \
	   src/configuration/server.cpp \
	   src/configuration/ConfigServer.cpp \
	   src/networking/ManageServers.cpp \
	   src/networking/Client.cpp \
	   src/request/Request.cpp \

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

.cpp.o: ${SRCS}
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run