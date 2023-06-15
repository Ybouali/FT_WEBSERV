#include "Response.hpp"

Response::Response() : statusCode(0), readBytes(0), readStatus(false), connectionStatus(false), sendStatus(false)
{
	this->body = "";
	this->responseContent = "";
	this->method = "";
	this->fullPath = "";
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
	this->body.clear();
	this->location.clear();
	this->method.clear();
	this->fullPath.clear();
	this->readBytes = 0;
	this->readStatus = false;
	this->connectionStatus = false;
	this->sendStatus = false;
}

// ----------------------------- Getters -----------------------------------

const ConfigServer&	Response::getConfigServer() const { return this->server; }

const Request&		Response::getRequest() const { return this->request; }

const std::string&	Response::getResponseContent() const { return this->responseContent; }

short				Response::getStatusCode() const { return this->statusCode; }

const std::string&	Response::getBody() const { return this->body; }

const Location&		Response::getLocation() const { return this->location; }

const std::string&	Response::getMethod() const { return this->method; }

const std::string&	Response::getFullPath() const { return this->fullPath; }

int					Response::getReadBytes() const { return this->readBytes; }

bool				Response::getReadStatus() const { return this->readStatus; }

bool				Response::getConnectionStatus() const { return this->connectionStatus; }

bool				Response::getSendStatus() const { return this->sendStatus; }

// ----------------------------- Setters -----------------------------------

void	Response::setConfigServer(const ConfigServer& server) { this->server = server; }

void	Response::setRequest(const Request& request) { this->request = request; }

void	Response::setResponseContent(const std::string& responseContent) { this->responseContent = responseContent; }

void	Response::setStatusCode(const int statusCode) { this->statusCode = statusCode; }

void	Response::setBody(const std::string& body) { this->body = body; }

void	Response::setLocation(const Location& location) { this->location = location; }

void	Response::setMethod(const std::string& method) { this->method = method; }

void	Response::setFullPath(const std::string& fullPath) { this->fullPath = fullPath; }

void	Response::setReadBytes(const int readBytes) { this->readBytes = readBytes; }

void	Response::setReadStatus(const bool readStatus) { this->readStatus = readStatus; }

void	Response::setConnectionStatus(const bool connectionStatus) { this->connectionStatus = connectionStatus; }

void	Response::setSendStatus(const bool sendStatus) { this->sendStatus = sendStatus; }

// ----------------------------- Methodes -----------------------------------

void	Response::buildResponse()
{
	try
	{
		// set the response status code based on the request code error
		// if the request is valid, the code error will be 0
		this->statusCode = this->request.getCodeError();
		if (this->statusCode)
		{
			throw std::exception();
		}

		// check if the request is valid
		this->isLocationMatched();
		this->isRedirectionExist();
		this->isMethodAllowed();

		// handle the request based on the method type (GET, POST, DELETE)
		if (this->method == "GET")
		{
			this->handleGetMethod();
		}  
		else if (this->method == "POST")
		{
			this->handlePostMethod();
		}
		else if (this->method == "DELETE")
		{
			this->handleDeleteMethod();
		}
	}
	catch(const std::exception& e)
	{
		if (this->statusCode == 301)
		{
			this->responseContent = getResponsePage(this->statusCode, true, this->server.getErrorPages().find(this->statusCode)->second);
			this->responseContent.append("Location: ");
			this->responseContent.append(this->fullPath);
			this->responseContent.append("\r\n");
		}
		else
		{
			this->responseContent = getResponsePage(this->statusCode, false, this->server.getErrorPages().find(this->statusCode)->second);
		}
	}
}

void	Response::buildResponseContent()
{
	this->responseContent = getResponsePage(this->statusCode, true, this->server.getErrorPages().find(this->statusCode)->second);
	this->responseContent.append("Content-Type: ");
	this->responseContent.append(getContentType(this->fullPath));
	this->responseContent.append("\r\n");
	this->responseContent.append("Content-Length: ");
	if (!this->body.empty())
	{
		this->responseContent.append(std::to_string(this->body.length()));
		this->responseContent.append("\r\n\r\n");
		this->responseContent.append(this->body);
	}
	else
	{
		struct stat st;
		const char *filename = this->fullPath.c_str();
		stat(filename, &st);
		off_t size = st.st_size;

		this->responseContent.append(std::to_string(size));
		this->responseContent.append("\r\n");
		this->responseContent.append("Connection: keep-alive");
		this->responseContent.append("\r\n\r\n");
	}
}

void	Response::isLocationMatched()
{
	std::vector<Location >	locations = this->server.getLocationList();
	std::string				requestedLocation = this->request.getPath();
	bool					isMatched = false;

	// if the URI has multiple slashes, erase the part after the first slash to get the requested location
	// example: /test/test2/test3 -> /test
	size_t pos = requestedLocation.find_first_of('/', 1);
	if (pos != std::string::npos)
	{
		requestedLocation.erase(pos);
	}

	// check if the requested location is matched with a location in the server
	for (std::vector<Location >::iterator it = locations.begin(); it != locations.end(); it++)
	{
		// if the requested location is matched, set it to the response location
		if (it->getLocation() == requestedLocation)
		{
			isMatched = true;
			this->location = *it;
		}
	}

	// if the requested location is not matched, set the status code to 404
	if (!isMatched)
	{
		this->statusCode = 404;
		throw std::exception();
	}
}

void	Response::isRedirectionExist()
{
	// check if the location has a redirection
	if (!location.getRedirection().empty())
	{
		// if it does, redirect the client to the redirection path
		this->statusCode = 301;
		this->fullPath = location.getRedirection();
		throw std::exception();
	}
}

void	Response::isMethodAllowed()
{
	std::vector<std::string>	allowedMethods = this->location.getMethod();
	std::string					requestedMethod = this->request.getMethodsString();
	bool						isAllowed = false;

	// check if the requested method is allowed
	for (std::vector<std::string>::iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++)
	{
		// if the requested method is allowed, set it to the response method
		if (*it == requestedMethod)
		{
			isAllowed = true;
			this->method = *it;
		}
	}

	// if the requested method is not allowed, set the status code to 405
	if (!isAllowed)
	{
		this->statusCode = 405;
		throw std::exception();
	}
}

void	Response::isResourceExist()
{
	// set the full path to the requested path and replace the location path with the root path
	this->fullPath = this->request.getPath();
	this->fullPath.replace(0, this->location.getLocation().length(), this->location.getRoot());

	// check if the requested resource exist
	if (access(this->fullPath.c_str(), F_OK) == -1)
	{
		this->statusCode = 404;
		throw std::exception();
	}
}
