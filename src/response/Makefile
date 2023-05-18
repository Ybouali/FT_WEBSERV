Name=webserv

src=main.cpp Response.cpp

Flags= -Wall -Wextra -Werror -std=c++98

all : $(Name)

$(Name): $(src)
	@c++ $(src) $(Flags) -o $(Name)
	@echo "WebServ On"

fclean : 
	rm -rf $(Name)

re : fclean all