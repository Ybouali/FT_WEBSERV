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
	try
	{
		this->isMethodSupported();
		this->isPathTooLong();
		this->isPathForbidden();
		this->isBodyTooLarge();
	}
	catch(const std::exception& e)
	{
		this->createErrorResponse();
	}

	this->createResponseContent();
}

void	Response::createResponseContent()
{
	std::string	statusCode = std::to_string(this->statusCode);

	this->responseContent = 
	"HTTP/1.1 " + statusCode + " " + this->statusMessage + "\r\n" +
	"Date: " + this->date + "\r\n" +
	"Server: nginx/13.37" + "\r\n" +
	"Content-Type: " + this->contentType + "\r\n" +
	"Content-Length: " + this->contentLength + "\r\n\r\n" +
	this->body + "\r\n";
}

void	Response::createErrorResponse()
{
	this->date = getDateFormat();
	this->statusMessage = statusCodeString(this->statusCode);
	this->body = getPageError(this->statusCode);
	this->contentType = "text/html";
	this->contentLength = std::to_string(this->body.length());
}

void	Response::isMethodSupported()
{
	if (this->request.getMethodsString() != "GET" &&
		this->request.getMethodsString() != "POST" &&
		this->request.getMethodsString() != "DELETE")
	{
		this->statusCode = 501;
		throw std::exception();
	}
}

void	Response::isPathTooLong()
{
	if (this->request.getPath().length() > MAX_URI_LENGTH)
	{
		this->statusCode = 414;
		throw std::exception();
	}
}

void	Response::isPathForbidden()
{
	for (size_t i = 0; i < this->request.getPath().length(); i++)
	{
		if (!checkUriCharacters(this->request.getPath().at(i)))
		{
			this->statusCode = 400;
			throw std::exception();
		}
	}
}

void	Response::isBodyTooLarge()
{
	if (this->request.getBody().length() > CLIENT_MAX_BODY_SIZE)
	{
		this->statusCode = 413;
		throw std::exception();
	}
}
