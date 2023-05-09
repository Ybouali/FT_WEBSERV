#include "server.hpp"

int main (int ac, char **av){
    std::string filename;
    if (ac == 2){
        server c;
        location loc;
        filename = av[1];
        c.parse_config_file(filename, c, loc);
        c.print_server_elements();
        loc.print_location_elements();
    }
}

