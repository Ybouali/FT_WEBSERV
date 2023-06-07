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
	this->body.clear();
	this->location.clear();
	this->method.clear();
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

// ----------------------------- Setters -----------------------------------

void	Response::setConfigServer(const ConfigServer& server) { this->server = server; }

void	Response::setRequest(const Request& request) { this->request = request; }

void	Response::setResponseContent(const std::string& responseContent) { this->responseContent = responseContent; }

void	Response::setStatusCode(const int statusCode) { this->statusCode = statusCode; }

void	Response::setBody(const std::string& body) { this->body = body; }

void	Response::setLocation(const Location& location) { this->location = location; }

void	Response::setMethod(const std::string& method) { this->method = method; }

void	Response::setFullPath(const std::string& fullPath) { this->fullPath = fullPath; }

// ----------------------------- Methodes -----------------------------------

void	Response::buildResponse()
{
	try
	{
		this->isLocationMatched();
		this->isRedirectionExist();
		this->isMethodAllowed();

		if (this->method == "GET")
			this->buildGetMethod();
		else if (this->method == "POST")
			this->buildPostMethod();
		else if (this->method == "DELETE")
			this->buildDeleteMethod();
	}
	catch(const std::exception& e)
	{
		this->responseContent = getResponsePage(this->statusCode, true, this->server.getErrorPages().find(this->statusCode)->second);
	}

}

void	Response::isLocationMatched()
{
	std::vector<Location *>	locations = this->server.getLocationList();
	std::string				requestedLocation = this->request.getPath();
	bool					isMatched = false;

	// If the URI have multiple slashes, erase the part after the first slash
	// Example: /test/test2/test3 -> /test
	size_t pos = requestedLocation.find_first_of('/', 1);
	if (pos != std::string::npos)
	{
		requestedLocation.erase(pos);
	}

	// Check if the requested location match with a location in the server
	for (std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		// If the requested location is matched, set it to the response location
		if ((*it)->getLocation() == requestedLocation)
		{
			isMatched = true;
			this->location = *(*it);
		}
	}

	// If the requested location is not matched, set the status code to 404
	if (!isMatched)
	{
		this->statusCode = 404;
		throw std::exception();
	}
}

void	Response::isRedirectionExist()
{
	// still not sure if this is the right way to do it ???

	// If the redirection exist, set the status code to 301
	if (!location.getRedirection().empty())
	{
		this->statusCode = 301;
		throw std::exception();
	}
}

void	Response::isMethodAllowed()
{
	std::vector<std::string>	allowedMethods = this->location.getMethod();
	std::string					requestedMethod = this->request.getMethodsString();
	bool						isAllowed = false;

	// Check if the requested method is allowed
	for (std::vector<std::string>::iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++)
	{
		// If the requested method is allowed, set it to the response method
		if (*it == requestedMethod)
		{
			isAllowed = true;
			this->method = *it;
		}
	}

	// If the requested method is not allowed, set the status code to 405
	if (!isAllowed)
	{
		this->statusCode = 405;
		throw std::exception();
	}
}

void	Response::buildGetMethod()
{
	// set the full path to the requested path
	this->fullPath = this->request.getPath();

	// replace the location path with the root path
	this->fullPath.replace(0, this->location.getLocation().length(), this->location.getRoot());

	// check if the requested resource exist
	if (access(this->fullPath.c_str(), F_OK) == -1)
	{
		this->statusCode = 404;
		throw std::exception();
	}

	// check if the requested resource is a directory or a file
	if (isDirectory(this->fullPath))
	{
		// TODO later
	}
	else
	{
		// check if the requested file type is supported
		if (!isTypeSupported(this->fullPath))
		{
			this->statusCode = 415;
			throw std::exception();
		}

		// check if the requested file is readable
		if (access(this->fullPath.c_str(), R_OK) == -1)
		{
			this->statusCode = 403;
			throw std::exception();
		}

		// check if the location have CGI
		if (this->location.getCgi() == "off")
		{
			// open the file and check if it's open
			std::ifstream	file(this->fullPath.c_str());
			if (!file.is_open())
			{
				this->statusCode = 500;
				throw std::exception();
			}

			// read the file content and store it in the body
			std::stringstream	buffer;
			buffer << file.rdbuf();
			this->body = buffer.str();
			file.close();

			// build the response content
			this->statusCode = 200;
			this->responseContent = getResponsePage(this->statusCode, false, this->server.getErrorPages().find(this->statusCode)->second);
			this->responseContent.append("Content-Type: ");
			this->responseContent.append(getContentType(this->fullPath));
			this->responseContent.append("\r\n");
			this->responseContent.append("Content-Length: ");
			this->responseContent.append(std::to_string(body.length()));
			this->responseContent.append("\r\n\r\n");
			this->responseContent.append(this->body);
		}
		else if (this->location.getCgi() == "on")
		{
			// TODO: CGI
		}
	}
}

void	Response::buildPostMethod()
{
	// TODO: later
}

void	Response::buildDeleteMethod()
{
	// TODO: later
}
