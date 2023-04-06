# pragma once

# include "../webserv.hpp"

class ConfigServer;
class Client;

class ManageServers {
    private :
        std::vector<ConfigServer>           Servers;
        std::map<int, ConfigServer>         serversMap;
        fd_set                              recvFd;
        fd_set                              writeFd;
        int                                 biggestFd; // ? manage client connections,
        std::map<int, Client>               clientsMap;

    public : 
        ManageServers();

        ~ManageServers();

        void                                clear();

        //! ----------------------------- getters -----------------------------------

        std::vector<ConfigServer>           getServers() const;

        std::map<int, ConfigServer>         getServersMap() const;

        fd_set                              getRecvFd() const;

        fd_set                              getWriteFd() const;

        std::map<int, Client>               getClient() const;

        //! ----------------------------- setters -----------------------------------

        void                            setServers(std::vector<ConfigServer>);

        void                            setServersMap(std::map<int, ConfigServer>);

        void                            setRecvFd(fd_set);

        void                            setWriteFd(fd_set);

        void                            setClient(std::map<int, Client>);
};