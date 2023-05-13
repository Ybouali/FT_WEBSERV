#include "ConfigServer.hpp"

ConfigServer::ConfigServer()
    : Port(), serverName("exempl.com"), Root("www/"), Index("index.html"), errorPages(), clientMaxBodySize(CLIENT_MAX_BODY_SIZE)
{
    this->setPort("8080");
    this->setHost("127.0.0.1");
}

ConfigServer::ConfigServer(std::string port, std::string host, std::string ServerName, std::string root, unsigned long ClientMaxBodySize, std::string index)
    : Port(0), Host(), serverName(), Root(""),  Index(""), errorPages(), clientMaxBodySize(0)
{
    this->setPort(port);
    this->setHost(host);
    this->setServerName(ServerName);
    this->setRoot(root);
    this->setClientMaxBodySize(ClientMaxBodySize);
    this->setIndex(index);
    this->initErrorPages();
    this->setFd(0);
}

ConfigServer::ConfigServer(const ConfigServer & other) { *this = other; }


ConfigServer & ConfigServer::operator=(const ConfigServer & other)
{
    if (this != &other)
	{
        this->Port = other.Port;
        this->Host = other.Host;
        this->serverName = other.serverName;
        this->Root = other.Root;
        this->clientMaxBodySize = other.clientMaxBodySize;
        this->Index = other.Index;
        this->errorPages = other.errorPages;
        this->Fd = other.Fd;
        this->serverAddress = other.serverAddress;
        // this->locations = other.locations;
    }
    return *this;
}



ConfigServer::~ConfigServer() { clear(); }

void        ConfigServer::clear()
{
    this->Port = 0;
    this->Host = 0;
    this->serverName.clear();
    this->Root.clear();
    this->clientMaxBodySize = 0;
    this->Index.clear();
    this->errorPages.clear();
}

void                                ConfigServer::initErrorPages()
{
    this->errorPages[301] = "";
	this->errorPages[302] = "";
	this->errorPages[400] = "";
	this->errorPages[401] = "";
	this->errorPages[402] = "";
	this->errorPages[403] = "";
	this->errorPages[404] = "";
	this->errorPages[405] = "";
	this->errorPages[406] = "";
	this->errorPages[500] = "";
	this->errorPages[501] = "";
	this->errorPages[502] = "";
	this->errorPages[503] = "";
	this->errorPages[505] = "";
	this->errorPages[505] = "";
}

//! ----------------------------- getters -----------------------------------

uint16_t                            ConfigServer::getPort() const { return this->Port; }

in_addr_t                           ConfigServer::getHost() const { return this->Host; }

std::string                         ConfigServer::getServerName() const { return this->serverName; }

std::string                         ConfigServer::getRoot() const { return this->Root; }

unsigned long                       ConfigServer::getClientMaxBodySize() const { return this->clientMaxBodySize; }

std::string                         ConfigServer::getIndex() const { return this->Index; }

std::map<short, std::string>        ConfigServer::getErrorPages() const { return this->errorPages; }

int                                 ConfigServer::getFd() const { return this->Fd; }

//! ----------------------------- setters -----------------------------------

void                ConfigServer::setPort(std::string port) { this->Port = static_cast<uint16_t>(std::stoul(port)); }

void                ConfigServer::setHost(std::string host) { this->Host = inet_addr(host.c_str());/* ! If host == INADDR_NONE so this is an error (Failed to get the host for localhost) .*/ }

void                ConfigServer::setServerName(std::string ServerName) { this->serverName = ServerName; }

void                ConfigServer::setRoot(std::string root) { this->Root = root; }

void                ConfigServer::setClientMaxBodySize(unsigned long size) { this->clientMaxBodySize = size; }

void                ConfigServer::setIndex(std::string index) { this->Index = index; }

void                ConfigServer::setErrorPages(std::map<short, std::string> ErrorPages) { this->errorPages = ErrorPages; }

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
    this->serverAddress.sin_addr.s_addr = this->getHost();
    this->serverAddress.sin_port = htons(this->getPort());
    
    // ! Here bind address with the port 
    if (bind(this->Fd, (struct sockaddr *) &this->serverAddress, sizeof(this->serverAddress)) < 0)
    {
        std::cerr << "PORT [" << this->getPort() << "] SERVER NAME [" << this->getServerName() << "]" << std::endl;
        std::cerr << "webserver: bind error [" << strerror(errno) << "]" << std::endl;
        exit(EXIT_FAILURE);
    }
}