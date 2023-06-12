# include "Client.hpp"

Client::Client() 
    : clientSocket(), lastMsgTime(time(NULL)), clientAddress(), request(), server()
{
}

Client::~Client()
{ }

Client::Client(const ConfigServer& server)
    : clientSocket(), lastMsgTime(time(NULL)), clientAddress(), request(), server()
{ 
    this->server = server;
    this->request.setMaxBodySize(server.getClientMaxBodySize());
    this->lastMsgTime = time(NULL);
}

void                        Client::buildResponse()
{
    // // ! FOR TESTING
    // if (this->request.getMethodsString() == "POST" && !this->request.getCodeError())
    // {
    //     if (this->request.getBody().size() > this->server.getClientMaxBodySize())
    //     {
    //         this->request.setCodeError(413);
    //         return ;
    //     }
    //     // In the second argument of this function we can set the name file 
    //     if (!this->request.getCodeError())
    //         this->request.uploadFile("www/uploads/");
    // }

    if (!this->request.getCodeError())
    {
        this->response.setConfigServer(this->server);
        this->response.setRequest(this->request);
        this->response.buildResponse();
    }
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
    this->response.clear();
}

