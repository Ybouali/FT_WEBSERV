#include "server.hpp"
int main(int ac, char **av){
    (void)ac;
    std::vector<server *>   servers;
    std::string filename = av[1];
    server s;
    servers = s.get_server(filename);

    for (size_t i = 0; i < servers.size(); i++)
    {
        std::cout << "|||||||||||||||||||||||||||||||||||||||| START PRINTING SRVER NUMBER [" << i + 1 << "] ||||||||||||||||||||||||||||||" << std::endl;
        servers[i]->printTheServerInfo();
        std::cout << "|||||||||||||||||||||||||||||||||||||||| END PRINTING SRVER NUMBER   [" << i + 1 << "] ||||||||||||||||||||||||||||||" << std::endl;
    }

























    // std::cout << "NUMBER OF SERVER [" << servers.size() << "]" << std::endl;
    
    // std::cout << "NUMBER OF LOCATIONS {" << servers[0]->get_locations().size() << "}" << std::endl;
    
}