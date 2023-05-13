# include "ManageServers.hpp"

ManageServers::ManageServers()
    : Servers(), serversMap(), readFd(), writeFd(), biggestFd(), clientsMap()
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
            close(Servers[i].getFd());
        this->Servers.clear();
    }
    if (!this->serversMap.empty())
    {
        for (std::map<int, ConfigServer>::iterator it = this->serversMap.begin(); it != this->serversMap.end(); ++it)
            close (it->second.getFd());
        this->serversMap.clear();
    }

    if (!this->clientsMap.empty()) 
        this->clientsMap.clear();
    this->biggestFd = 0;
}

//! ----------------------------- getters -----------------------------------

std::vector<ConfigServer>       ManageServers::getServers() const { return this->Servers; }

std::map<int, ConfigServer>     ManageServers::getServersMap() const { return this->serversMap; }

fd_set                          ManageServers::getreadFd() const { return this->readFd; }

fd_set                          ManageServers::getWriteFd() const { return this->writeFd; }

std::map<int, Client>           ManageServers::getClient() const { return this->clientsMap; }

//! ----------------------------- setters -----------------------------------

void                            ManageServers::setServers(std::vector<ConfigServer> servers) { this->Servers = servers; }

void                            ManageServers::setServersMap(std::map<int, ConfigServer> ServersMap) { this->serversMap = ServersMap; }

void                            ManageServers::setreadFd(fd_set Recv) { this->readFd = Recv; }

void                            ManageServers::setWriteFd(fd_set Write) { this->writeFd = Write; }

void                            ManageServers::setClient(std::map<int, Client> Clients) { this->clientsMap = Clients; }

//! Methods ----------------------------------------------------------------


void                            ManageServers::acceptClientConnection(ConfigServer& server)
{
    struct  sockaddr_in         clientAddress;
    long                        clientAddressSize = sizeof(clientAddress);
    int                         clientSock;
    Client                      client(server);

    clientSock = accept(server.getFd(), (struct sockaddr *)&clientAddress, (socklen_t*)&clientAddressSize);
    if (clientSock == -1)
    {
        std::cerr << "Failed to accept " << std::endl;
        return;
    }
    this->addToSet(clientSock, this->readFd);
    
    if (fcntl(clientSock, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "webserver: fcntl error [" << strerror(errno) << "]" << std::endl;
        removeFromSet(clientSock, this->readFd);
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
    int     r_recv = 0;

    r_recv = recv(i, buf, MSG_BUF, 0);
    if (r_recv == 0)
    {
        std::cerr << "webserv: Client " << i << " Closed Connection" << std::endl;
        this->closeConnectionClient(i);
        return ;
    }
    else if (r_recv < 0)
    {
        std::cerr << "webserv: Fd " << i << " read error " << strerror(errno) << std::endl;
        this->closeConnectionClient(i);
        return ;
    }
    else if (r_recv != 0)
    {
        client.updateTime();
        client.request.readBufferFromReq(buf, r_recv);
        memset(buf, 0, sizeof(buf));
    }
    if (client.request.getState() == Parsing_Done || client.request.getCodeError())
    {
        this->assignServerToClient(client);

        // TODO: need a build Response for the client based on the request object
        // ! and also handle the the cgi if exists
        

        // move fd from recv fd and add it to the write fd
        this->removeFromSet(i, this->readFd);
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

void                            ManageServers::initSets()
{
    FD_ZERO(&this->readFd);
    FD_ZERO(&this->writeFd);

    for(std::vector<ConfigServer>::iterator it = this->Servers.begin(); it != this->Servers.end(); ++it)
    {
        if (listen(it->getFd(), 512) == -1)
        {
            std::cerr << "webserv: listen error: [" << strerror(errno) << "] Closing" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0)
        {
            std::cerr << "webserv: fcntl error: [" << strerror(errno) << "] Closing" << std::endl;
            exit(EXIT_FAILURE);
        }
        addToSet(it->getFd(), this->readFd);
        this->serversMap.insert(std::make_pair(it->getFd(), *it));
    }
    this->biggestFd = this->Servers.back().getFd();
}

void                            ManageServers::setupServers(std::vector<ConfigServer> servers)
{
    bool            checkDoubleServers;

    this->Servers = servers;
    if (this->Servers.empty())
    {
        this->Servers.push_back(ConfigServer());
        std::cout << "[INFO]: There is no server in the config file or there is no config file " << std::endl;
        std::cout << "[INFO]: So the Host [127.0.0.1] and the post will be [" << this->Servers.at(0).getPort() << "]" << std::endl;
    }

    for (std::vector<ConfigServer>::iterator it = this->Servers.begin(); it != this->Servers.end(); ++it)
    {
        checkDoubleServers = false;
        for (std::vector<ConfigServer>::iterator it2 = this->Servers.begin(); it2 != it; ++it2)
        {
            if (it2->getHost() == it->getHost() && it2->getPort() == it->getPort())
            {
                it->setFd(it2->getFd());
                checkDoubleServers = true;
            }
        }
        if (!checkDoubleServers)
            it->setupServer();
    }
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
    if (FD_ISSET(i, &this->readFd))
        this->removeFromSet(i, this->readFd);
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

void                            ManageServers::startServers()
{
    fd_set  readCpy;
    fd_set  writeCpy;
    int     r_select;

    this->biggestFd = 0;
    this->initSets();
    struct timeval timer;

    std::cout << "--------------------------- STARTING --------------------------------" << std::endl;
    
    while (true)
    {
        timer.tv_sec = 1;
        timer.tv_usec = 0;
        readCpy = this->readFd;
        writeCpy = this->writeFd;

        if ( (r_select = select(this->biggestFd + 1, &readCpy, &writeCpy, NULL, &timer)) < 0 )
        {
            std::cerr << "Webserv error :: select [" << strerror(errno) << "] Closing !" << std::endl;
            exit(1);
        }
        for (int i = 0; i <= this->biggestFd; ++i)
        {
            if (FD_ISSET(i, &readCpy) && this->serversMap.count(i))
                this->acceptClientConnection(this->serversMap.find(i)->second);
            else if (FD_ISSET(i, &readCpy) && this->clientsMap.count(i))
            {
                this->readRequest(i, this->clientsMap[i]);
                // this->clientsMap[i].request.printRequest(i);
            }
            else if (FD_ISSET(i, &writeCpy))
                this->sendRes(i, this->clientsMap[i]);
        }
        this->timeoutCheck();
    }
}

void                            ManageServers::sendRes(const int & i, Client & client)
{
    int sentBytes;
    std::string response = "";
    // TODO: here the headers is static it should be dynamic !! 
    
    //     response = "HTTP/1.1 501 Not Implemented\r\n";
    //     response.append("Content-Type: text/html\r\n");
    //     // response.append("Content-Length: 113\r\n");
    //     response.append("Server: small_nginx\r\n");
    //     response.append("Date: Sun, 07 May 2023 20:30:06 UTC\r\n\r\n");
    
    // // ! FOR_TESTING:
    // if (client.request.getCodeError() == 0)
    //     client.request.setCodeError(200);

    // std::string errorMessage = getPageError(client.request.getCodeError());
    
    // if (!errorMessage.empty())
    //     response.append(errorMessage);

    if (response.size() >= MSG_BUF)
        sentBytes = send(i, response.c_str(), MSG_BUF, 0);
    else
        sentBytes = send(i, response.c_str(), response.size(), 0);

    if (sentBytes < 0) 
    {
        std::cerr << "sendRes(): error sending : " << strerror(errno) << std::endl;
        this->closeConnectionClient(i);
    }
    else if (sentBytes == 0 || (size_t) sentBytes == response.length())
    {
        // TODO: here need to check for the cgi from the client response if exists
        if (client.request.keepAlive() == false || client.request.getCodeError() )
        {
            std::cerr << "Client [" << i << "] Connection Closed" << std::endl;
            this->closeConnectionClient(i);
        }
        else
        {
            this->removeFromSet(i, this->writeFd);
            this->addToSet(i, this->readFd);
            client.clear();
        }
    }
    else
    {
        client.updateTime();
        // TODO: here need to remove from the response the sentBytes that already sends [headers, body ]
    }
}