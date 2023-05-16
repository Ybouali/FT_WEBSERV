#include "server.hpp"
int main(int ac, char **av){
    (void)ac;
    std::vector<server *>   servers;
    std::string filename = av[1];
    server s;
    servers = s.get_server(filename);
    std::cout << "NUMBER OF SERVER [" << servers.size() << "]" << std::endl;
    
    std::cout << "NUMBER OF LOCATIONS {" << servers[0]->get_locations().size() << "}" << std::endl;
    
}