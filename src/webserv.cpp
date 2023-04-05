#include "webserv.hpp"

int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    // ! WHAT NEEDS BY THE SERVERS FROM THE CONFIG FILE

    // ! Create A Class that contains this variables
    // ! and it should be a vector of this class SO that means every servers will be a object of this class
    /*
        uint16_t						    Port;
		in_addr_t						    Host;
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

   std::cout << "WEBSERV....." << std::endl;

   std::vector<ConfigServer> vecServers;
   ConfigServer server1 = ConfigServer(80, "127.0.0.1", "exemple.com", "www/", CLIENT_MAX_BODY_SIZE, "index.html", false);
   ConfigServer server2 = ConfigServer(81, "127.0.0.1", "exemple.com", "www/", CLIENT_MAX_BODY_SIZE, "index.html", false);
   vecServers.push_back(server1);
   vecServers.push_back(server2);

   for (size_t i = 0; i < vecServers.size(); i++)
    close(vecServers[i].getListenFd());
   
   
    return 0;
}