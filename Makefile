
NAME = webserv

CC = clang++

FLAGS = --std=c++98 -Wall -Wextra -Werror -fsanitize=address 

SRCS = webserv.cpp

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

run: 
	@make fclean && make && make clean && ./webserv 2>/dev/null

.PHONY: all clean fclean re run