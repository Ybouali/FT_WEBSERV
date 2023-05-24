#include "Response.hpp"

Response::Response() : statusCode(0)
{
}

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

const ConfigServer&							Response::getConfigServer() const { return this->server; }

const Request&								Response::getRequest() const { return this->request; }

const std::string&							Response::getResponseContent() const { return this->responseContent; }

short										Response::getStatusCode() const { return this->statusCode; }

const std::string&							Response::getStatusMessage() const { return this->statusMessage; }

const std::string&							Response::getContentType() const { return this->contentType; }

const std::string&							Response::getDate() const { return this->date; }

const std::string&							Response::getContentLength() const { return this->contentLength; }

const std::string&							Response::getBody() const { return this->body; }

const std::vector<Location *>::iterator&	Response::getItLocation() const { return this->itLocation; }

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

void	Response::setItLocation(const std::vector<Location *>::iterator& itLocation) { this->itLocation = itLocation; }

// ----------------------------- Methodes -----------------------------------

void	Response::buildResponse()
{
	try
	{
		this->isLocationMatched();
	}
	catch(const std::exception& e)
	{
		this->responseContent = getPageErrorWithHeaders(this->statusCode, true, this->server.getErrorPages().find(this->statusCode)->second);
	}

}

void	Response::isLocationMatched()
{
	std::vector<Location *>	locations = this->server.getLocationList();
	std::string				requestLocation = this->request.getPath();
	bool					isMatched = false;

	// If the URI have multiple slashes, we erase the part after the first slash
	// Example: /test/test2/test3 -> /test
	size_t pos = requestLocation.find_first_of('/', 1);
	if (pos != std::string::npos)
	{
		requestLocation.erase(pos);
	}

	// We check if the request location match with a location in the server
	for (std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		// If the location is matched, we set the iterator to the location
		if ((*it)->getLocation() == requestLocation)
		{
			isMatched = true;
			this->itLocation = it;
		}
	}

	// If the location is not matched, we set the status code to 404
	if (!isMatched)
	{
		this->statusCode = 404;
		throw std::exception();
	}
}
