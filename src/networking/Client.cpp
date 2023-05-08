# include "Client.hpp"

Client::Client() 
    : clientSocket(), lastMsgTime(time(NULL)), clientAddress(), request(), server()
{
}

Client::~Client()
{ }

Client::Client(const ConfigServer& other) { 
    // here we need to make sure that the server is initialized
    this->server = other;
    this->request.setMaxBodySize(server.getClientMaxBodySize());
    this->lastMsgTime = time(NULL);
 }

//! ----------------------------- getters -----------------------------------

int                     Client::getClientSocket() const { return this->clientSocket; }

time_t                  Client::getLastMsgTime() const { return this->lastMsgTime; }

struct sockaddr_in      Client::getClientAddress() const { return this->clientAddress; }

//! ----------------------------- setters -----------------------------------

void                    Client::setClientSocket(int socket) { this->clientSocket = socket; }

void                    Client::setLastMsgTime(time_t time) { this->lastMsgTime = time; }

void                    Client::setClientAddress(struct sockaddr_in ClientAddress) { this->clientAddress = ClientAddress; }

// ? Methodes ----------------------------------------------------------------

void                    Client::updateTime() { this->setLastMsgTime(time(NULL)); }

void                    Client::clear()
{
    this->request.clear();
    // TODO: need to clear also the response 
}

