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

//! Methods ----------------------------------------------------------------


void                            ManageServers::acceptClientConnection(ConfigServer& server)
{
    struct  sockaddr_in         clientAddress;
    long                        clientAddressSize = sizeof(clientAddress);
    int                         clientSock;
    Client                      client(server);

    clientSock = accept(server.getListenFd(), (struct sockaddr *)&clientAddress, (socklen_t*)&clientAddressSize);
    if (clientSock == -1)
    {
        std::cerr << "Failed to accept" << std::endl;
        return;
    }
    
    this->addToSet(clientSock, this->recvFd);
    
    if (fcntl(clientSock, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "webserver: fcntl error [" << strerror(errno) << "]" << std::endl;
        removeFromSet(clientSock, this->recvFd);
        close(clientSock);
        return ;
    }

    client.setClientSocket(clientSock);
    if (this->clientsMap.count(clientSock) != 0)
        this->clientsMap.erase(clientSock);
    this->clientsMap.insert(std::make_pair(clientSock, client));
}

void                            ManageServers::readRequest(const int & i, Client & client)
{
    char    buf[MSG_BUF];
    int     r_read = 0;

    r_read = read(i, buf, MSG_BUF);
    if (r_read == 0)
    {
        std::cerr << "webserv: Client " << i << " Closed Connection" << std::endl;
        this->closeConnectionClient(i);
        return ;
    }
    else if (r_read < 0)
    {
        std::cerr << "webserv: Fd " << i << " read error " << strerror(errno) << std::endl;
        this->closeConnectionClient(i);
        return ;
    }
    else if (r_read != 0)
    {
        client.updateTime();
        client.request.readBufferFromReq(buf, r_read);
        memset(buf, 0, sizeof(buf));
    }
    if (client.request.getState() == Parsing_Done || client.request.getCodeError())
    {
        this->assignServerToClient(client);

        // TODO: need a build Response for the client based on the request object
        // ! and also handle the the cgi if exists
        

        // move fd from recv fd and add it to the write fd
        this->removeFromSet(i, this->recvFd);
        this->addToSet(i, this->writeFd);
    }
}

void                            ManageServers::assignServerToClient(Client & client)
{
    for (std::vector<ConfigServer>::iterator it = this->Servers.begin(); it != this->Servers.end(); ++it)
    {
        if (client.server.getHost() == it->getHost() && client.server.getPort() == it->getPort() && client.request.getServerName() == it->getServerName())
        {
            client.server = *it;
            return ;
        }
    }
}

void                            ManageServers::initializeSets()
{
    FD_ZERO(&this->recvFd);
    FD_ZERO(&this->writeFd);

    for(std::vector<ConfigServer>::iterator it = this->Servers.begin(); it != this->Servers.end(); ++it)
    {
        //Now it calles listen() twice on even if two servers have the same host:port
        if (listen(it->getListenFd(), 512) == -1)
        {
            std::cerr << "webserv: listen error: [" << strerror(errno) << "] Closing" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (fcntl(it->getListenFd(), F_SETFL, O_NONBLOCK) < 0)
        {
            std::cerr << "webserv: fcntl error: [" << strerror(errno) << "] Closing" << std::endl;
            exit(EXIT_FAILURE);
        }
        addToSet(it->getListenFd(), this->recvFd);
        this->serversMap.insert(std::make_pair(it->getListenFd(), *it));
    }
    this->biggestFd = this->Servers.back().getListenFd();
}

void                            ManageServers::timeoutCheck()
{
    for(std::map<int, Client>::iterator it = this->clientsMap.begin() ; it != this->clientsMap.end(); ++it)
    {
        if (time(NULL) - it->second.getLastMsgTime() > TIMEOUT_CONNECTION)
        {
            std::cerr << "Client [" << it->first << "] Timeout Connection, Closing ..." << std::endl;
            this->closeConnectionClient(it->first);
            return ;
        }
    }
}

void                            ManageServers::closeConnectionClient(const int i)
{
    if (FD_ISSET(i, &this->writeFd))
        this->removeFromSet(i, this->writeFd);
    if (FD_ISSET(i, &this->recvFd))
        removeFromSet(i, this->recvFd);
    close(i);
    this->clientsMap.erase(i);
}

void                            ManageServers::removeFromSet(const int i, fd_set & set)
{
    FD_CLR(i, &set);
    if (i == this->biggestFd)
        this->biggestFd--;
}

void                            ManageServers::addToSet(const int i, fd_set & set)
{
    FD_SET(i, &set);
    if (i > this->biggestFd)
        biggestFd = i;
}