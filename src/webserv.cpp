# include "webserv.hpp"
# include "networking/ManageServers.hpp"

int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    std::vector<ConfigServer> vecServers;

    ManageServers master;

   ConfigServer server1 = ConfigServer("8000", "127.0.0.1", "exemple.com", "www/", CLIENT_MAX_BODY_SIZE, "index.html");
   ConfigServer server2 = ConfigServer("8001", "127.0.0.1", "exemple.com", "www/", CLIENT_MAX_BODY_SIZE, "index.html");
   ConfigServer server3 = ConfigServer("8002", "127.0.0.1", "exemple.com", "www/", CLIENT_MAX_BODY_SIZE, "index.html");
   
   vecServers.push_back(server1);
   vecServers.push_back(server2);
   vecServers.push_back(server3);


    // ! To setup the servers the program will need a vector of ConfigServer
    master.setupServers(vecServers);
    master.startServers();
   
    return 0;
}