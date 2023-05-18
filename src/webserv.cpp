# include "webserv.hpp"
# include "networking/ManageServers.hpp"

int main(int ac, char **av)
{
    std::string                     filename;
    ServerParser                    serversFromConfigFile;
    std::vector<ServerParser *>     servers;
    ManageServers master;


    if (ac == 2)
    {
        filename = (ac == 2) ? av[1] : "";
        servers = serversFromConfigFile.get_server(filename);
    }

    master.setupServers(master.getInfoServer(servers));
    master.startServers();
   
    return 0;
}