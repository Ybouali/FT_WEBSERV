#include "ConfigServer.hpp"

ConfigServer::ConfigServer()
    : Port(), Host(), serverName("exempl.com"), errorPages(), clientMaxBodySize(CLIENT_MAX_BODY_SIZE), locationList()
{
    this->setPort("8080");
    this->setHost("127.0.0.1");
    Location *local = new Location();
    this->locationList.push_back(local);
}

ConfigServer::ConfigServer(std::string port, std::string host, std::string ServerName, std::vector<Location * > _locationList, std::map<short, std::string> errorPages, std::string clientMaxBodySize)
    : Port(), Host(), serverName(), errorPages(), clientMaxBodySize(), locationList()
{
    this->setPort(port);
    this->setHost(host);
    this->setServerName(ServerName);
    this->locationList = _locationList;
    this->errorPages = errorPages;
    this->setClientMaxBodySize(clientMaxBodySize);
    this->setFd(0);
}

ConfigServer::ConfigServer(const ConfigServer & other) {*this = other; }


ConfigServer & ConfigServer::operator=(const ConfigServer & other)
{
    if (this != &other)
	{
        this->Port = other.Port;
        this->Host = other.Host;
        this->serverName = other.serverName;
        this->clientMaxBodySize = other.clientMaxBodySize;
        this->errorPages = other.errorPages;
        this->Fd = other.Fd;
        this->serverAddress = other.serverAddress;
        this->locationList = other.locationList;
    }
    return *this;
}

ConfigServer::~ConfigServer() { clear(); }

void        ConfigServer::clear()
{
    this->Port = 0;
    this->Host.clear();
    this->serverName.clear();
    this->clientMaxBodySize = 0;
    this->errorPages.clear();
    this->locationList.clear();
}

//! ----------------------------- getters -----------------------------------

uint16_t                            ConfigServer::getPort() const { return this->Port; }

std::string                         ConfigServer::getHost() const { return this->Host; }

std::string                         ConfigServer::getServerName() const { return this->serverName; }

std::map<short, std::string>        ConfigServer::getErrorPages() const { return this->errorPages; }

unsigned long                       ConfigServer::getClientMaxBodySize() const { return this->clientMaxBodySize; }

std::vector<Location * >            ConfigServer::getLocationList() const { return this->locationList; }

int                                 ConfigServer::getFd() const { return this->Fd; }

//! ----------------------------- setters -----------------------------------

void                ConfigServer::setPort(std::string port) { this->Port = static_cast<uint16_t>(std::stoul(port)); }

void                ConfigServer::setHost(std::string host) { 
    // TODO: Need a check for the host
    this->Host = host;
}

void                ConfigServer::setServerName(std::string ServerName) { this->serverName = ServerName; }

void                ConfigServer::setErrorPages(std::map<short, std::string> ErrorPages) { this->errorPages = ErrorPages; }

void                ConfigServer::setClientMaxBodySize(std::string size) { 
    unsigned long _clientMaxBodySize = std::stoul(size);

    if (!_clientMaxBodySize)
        this->clientMaxBodySize = _clientMaxBodySize;
}

void                ConfigServer::setLocationList(std::vector<Location * > _locationList) { this->locationList = _locationList; }

void                ConfigServer::setFd(int fd) { this->Fd = fd; }

void                                ConfigServer::setupServer()
{
    // ! Create a fd socket 
    // ! The AF_INET is responsible for IPv4 connection 
    // ! And SOCK_STREAM is responsible for TCP connection
    this->Fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->Fd < 0)
    {
        std::cerr << "PORT [" << this->getPort() << "] SERVER NAME [" << this->getServerName() << "]" << std::endl;
        std::cerr << "webserver: socket error [" << strerror(errno) << "]" << std::endl;
        exit(EXIT_FAILURE);
    }

    int option_value = 1;
    
    // ! Here set the address local to reusable
    if (setsockopt(this->Fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) == -1)
    {
        std::cerr << "PORT [" << this->getPort() << "] SERVER NAME [" << this->getServerName() << "]" << std::endl;
        std::cerr << "webserver: setsockopt error [" << strerror(errno) << "]" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = inet_addr(this->getHost().c_str());
    this->serverAddress.sin_port = htons(this->getPort());
    
    // ! Here bind address with the port 
    if (bind(this->Fd, (struct sockaddr *) &this->serverAddress, sizeof(this->serverAddress)) < 0)
    {
        std::cerr << "PORT [" << this->getPort() << "] SERVER NAME [" << this->getServerName() << "]" << std::endl;
        std::cerr << "webserver: bind error [" << strerror(errno) << "]" << std::endl;
        exit(EXIT_FAILURE);
    }
}