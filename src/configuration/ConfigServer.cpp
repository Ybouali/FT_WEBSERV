#include "ConfigServer.hpp"

ConfigServer::ConfigServer(size_t numberServer)
    : Port(0), Host(0), serverName(), Root(), clientMaxBodySize(CLIENT_MAX_BODY_SIZE), Index(), autoIndex(false), errorPages()
{

    if (numberServer)
    {
        for (size_t i = 0; i < numberServer; i++)
        {
            
        }
        this->initErrorPages();
    }
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

//! ----------------------------- setters -----------------------------------

void                ConfigServer::setPort(uint16_t port) { this->Port = port; }

void                ConfigServer::setHost(in_addr_t host) { this->Host = host; }

void                ConfigServer::setServerName(std::string ServerName) { this->serverName = ServerName; }

void                ConfigServer::setRoot(std::string root) { this->Root = root; }

void                ConfigServer::setClientMaxBodySize(unsigned long size) { this->clientMaxBodySize = size; }

void                ConfigServer::setIndex(std::string index) { this->Index = index; }

void                ConfigServer::setAutoIndex(bool AutoIndex) { this->autoIndex = AutoIndex; }

void                ConfigServer::setErrorPages(std::map<short, std::string> ErrorPages) { this->errorPages = ErrorPages; }