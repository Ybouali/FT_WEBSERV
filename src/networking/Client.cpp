# include "Client.hpp"

Client::Client() 
    : clientSocket(), lastMsgTime(time(NULL)), clientAddress(), request(), server()
{
}

Client::~Client()
{
    this->clear();
}

Client::Client(const ConfigServer& server)
    : clientSocket(), lastMsgTime(time(NULL)), clientAddress(), request(), server()
{ 
    this->server = server;
    this->lastMsgTime = time(NULL);
}

void                        Client::buildResponse()
{
    if (this->request.getCodeError())
    {
        std::string errorPage = this->server.getErrorPages().find(this->request.getCodeError())->second;
        std::string responsePage = getResponsePage(this->request.getCodeError(), true, errorPage);

        this->response.setResponseContent(responsePage);
        this->response.setConnectionStatus(true);

        return ;
    }

    this->response.setConfigServer(this->server);
    this->response.setRequest(this->request);
    this->response.buildResponse();
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
    this->server.clear();
    this->request.clear();
    this->response.clear();
}
