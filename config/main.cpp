#include "server.hpp"
int main(int ac, char **av){
    (void)ac;
    std::string filename = av[1];
    server s;
    s.get_server(filename);
}