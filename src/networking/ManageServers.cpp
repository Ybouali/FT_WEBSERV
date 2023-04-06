# include "ManageServers.hpp"

ManageServers::ManageServers()
    : Servers(), serversMap(), recvFd(), writeFd(), biggestFd(), clientsMap()
{ }
        
ManageServers::~ManageServers()
{
    this->clear();
}

void ManageServers::clear()
{
    if (!this->Servers.empty())
    {
        for (size_t i = 0; i < Servers.size(); i++)
            close(Servers[i].getListenFd());
        this->Servers.clear();
    }
    if (!this->serversMap.empty())
    {
        for (std::map<int, ConfigServer>::iterator it = this->serversMap.begin(); it != this->serversMap.end(); ++it)
            close (it->second.getListenFd());
        this->serversMap.clear();
    }

    if (!this->clientsMap.empty()) 
        this->clientsMap.clear();
    this->biggestFd = 0;
}

//! ----------------------------- getters -----------------------------------

std::vector<ConfigServer>       ManageServers::getServers() const { return this->Servers; }

std::map<int, ConfigServer>     ManageServers::getServersMap() const { return this->serversMap; }

fd_set                          ManageServers::getRecvFd() const { return this->recvFd; }

fd_set                          ManageServers::getWriteFd() const { return this->writeFd; }

std::map<int, Client>           ManageServers::getClient() const { return this->clientsMap; }

//! ----------------------------- setters -----------------------------------

void                            ManageServers::setServers(std::vector<ConfigServer> servers) { this->Servers = servers; }

void                            ManageServers::setServersMap(std::map<int, ConfigServer> ServersMap) { this->serversMap = ServersMap; }

void                            ManageServers::setRecvFd(fd_set Recv) { this->recvFd = Recv; }

void                            ManageServers::setWriteFd(fd_set Write) { this->writeFd = Write; }

void                            ManageServers::setClient(std::map<int, Client> Clients) { this->clientsMap = Clients; }