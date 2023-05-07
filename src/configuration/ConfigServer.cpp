#include "ConfigServer.hpp"

ConfigServer::ConfigServer()
    : Port(80), serverName("exempl.com"), Root("www/"), clientMaxBodySize(CLIENT_MAX_BODY_SIZE), Index("index.html"), autoIndex(false), errorPages()
{
    this->setHost("127.0.0.1");
}

ConfigServer::ConfigServer(ConfigServer & other) { *this = other; }

ConfigServer & ConfigServer::operator=(ConfigServer & other)
{
    return *this;
}

ConfigServer::ConfigServer(uint16_t port, std::string host, std::string ServerName, std::string root, unsigned long ClientMaxBodySize, std::string index, bool AutoIndex)
    : Port(0), Host(), serverName(), Root(""), clientMaxBodySize(0), Index(""), autoIndex(false), errorPages()
{
    this->setPort(port);
    this->setHost(host);
    this->setServerName(ServerName);
    this->setRoot(root);
    this->setClientMaxBodySize(ClientMaxBodySize);
    this->setIndex(index);
    this->setAutoIndex(AutoIndex);
    this->initErrorPages();
    this->setListenFd();
    std::cout << "LISTEN FD :: [" << this->getListenFd() << "]" << std::endl;
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
    this->autoIndex = false;
    this->errorPages.clear();
}

// ! initialization of error pages

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

bool                                ConfigServer::getAutoIndex() const { return this->autoIndex; }

std::map<short, std::string>        ConfigServer::getErrorPages() const { return this->errorPages; }

int                                 ConfigServer::getListenFd() const { return this->listenFd; }

//! ----------------------------- setters -----------------------------------

void                ConfigServer::setPort(uint16_t port) { this->Port = port; }

void                ConfigServer::setHost(std::string host) { this->Host = inet_addr(host.c_str());/* ! If host == INADDR_NONE so this is an error (Failed to get the host for localhost) .*/ }

void                ConfigServer::setServerName(std::string ServerName) { this->serverName = ServerName; }

void                ConfigServer::setRoot(std::string root) { this->Root = root; }

void                ConfigServer::setClientMaxBodySize(unsigned long size) { this->clientMaxBodySize = size; }

void                ConfigServer::setIndex(std::string index) { this->Index = index; }

void                ConfigServer::setAutoIndex(bool AutoIndex) { this->autoIndex = AutoIndex; }

void                ConfigServer::setErrorPages(std::map<short, std::string> ErrorPages) { this->errorPages = ErrorPages; }

void                ConfigServer::setListenFd()
{
    this->listenFd = socket(AF_INET, SOCK_STREAM, 0); // ! check if the listen fd is not equal to -1 . exit EXIT_FAILURE

    int option_value = 1;
    setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = this->getHost();
    this->serverAddress.sin_port = htons(this->getPort());
    // ! bind the socket
    bind(this->listenFd, (struct sockaddr *) &this->serverAddress, sizeof(this->serverAddress)); // ! check if the listen fd is not equal to -1 . exit EXIT_FAILURE
}