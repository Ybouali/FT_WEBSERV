#include "server.hpp"

int main (int ac, char **av){
    
    // ! WHAT NEEDS BY THE SERVERS FROM THE CONFIG FILE

    // ! Create A Class that contains this variables
    // ! and it should be a vector of this class SO that means every servers will be a object of this class
    /*
        uint16_t						    Port;
		std::string						    Host;
		std::string						    Server_name;
		std::string						    Root;
		unsigned long					    clientMaxBodySize;
		std::string						    index;
		bool							    autoindex;
		std::vector<Location> 			    locations;
    */
   // ! Every location object should have 
    // std::string					Path;
    // std::string					Root;
    // bool						    Autoindex;
    // std::string					Index;
    // std::vector<short>			Methods; // GET+ POST- DELETE-
    // std::string					Return;
    // std::string					Alias;
    // std::vector<std::string>	    cgiPath;
    // std::vector<std::string>	    cgi_extention;
    // unsigned long				clientMaxBodySize;

    // !    NOTE: Here we should not stop the program if ac is not equal to two.
    // !    I will fix this issue.
    // !    Just you need to parse the file if exists.
    // !    And store the servers that u have found in a vector 

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

