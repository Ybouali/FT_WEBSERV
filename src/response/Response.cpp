#include "Response.hpp"

Response::Response() :
	server(),
	request(),
	responseContent(),
	statusCode(0),
	statusMessage(),
	date(),
	contentType(),
	contentLength(),
	body()
{}

Response::~Response()
{
    this->clear();
}

void	Response::clear()
{
    this->server.clear();
    this->request.clear();
	this->responseContent.clear();
	this->statusCode = 0;
	this->statusMessage.clear();
	this->date.clear();
	this->contentType.clear();
	this->contentLength.clear();
	this->body.clear();
}

// ----------------------------- Getters -----------------------------------

const ConfigServer&	Response::getConfigServer() const { return this->server; }

const Request&		Response::getRequest() const { return this->request; }

const std::string&	Response::getResponseContent() const { return this->responseContent; }

short				Response::getStatusCode() const { return this->statusCode; }

const std::string&	Response::getStatusMessage() const { return this->statusMessage; }

const std::string&	Response::getContentType() const { return this->contentType; }

const std::string&	Response::getDate() const { return this->date; }

const std::string&	Response::getContentLength() const { return this->contentLength; }

const std::string&	Response::getBody() const { return this->body; }

// ----------------------------- Setters -----------------------------------

void	Response::setConfigServer(const ConfigServer& server) { this->server = server; }

void	Response::setRequest(const Request& request) { this->request = request; }

void	Response::setResponseContent(const std::string& responseContent) { this->responseContent = responseContent; }

void	Response::setStatusCode(const int statusCode) { this->statusCode = statusCode; }

void	Response::setStatusMessage(const std::string& statusMessage) { this->statusMessage = statusMessage; }

void	Response::setDate(const std::string& Date) { this->date = Date; }

void	Response::setContentType(const std::string& contentType) { this->contentType = contentType; }

void	Response::setContentLength(const std::string& contentLength) { this->contentLength = contentLength; }

void	Response::setBody(const std::string& body) { this->body = body; }

// ----------------------------- Methodes -----------------------------------

void	Response::buildResponse()
{
}
