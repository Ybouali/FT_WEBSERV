#include "server.hpp"

int main (int ac, char **av){
    std::string filename;
    if (ac == 2){
        try {
            server c;
            location loc;
            filename = av[1];
            c.parse_config_file(filename, c, loc);
            c.print_server_elements();
            loc.print_location_elements();
            std::cout << "On Off == " << loc.getOnOff() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
