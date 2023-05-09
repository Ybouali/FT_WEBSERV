Name=webserv

src=main.cpp location.cpp server.cpp parse_location.cpp parse_server.cpp

Flags= -Wall -Wextra -Werror -std=c++98

all : $(Name)

$(Name): $(src)
	@c++ $(src) $(Flags) -o $(Name)
	@echo "WebServ On"

fclean : 
	rm -rf $(Name)

re : fclean all