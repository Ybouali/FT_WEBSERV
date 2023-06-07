#include "../Response.hpp"

void	Response::handleGetMethod()
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

	// check if the requested resource is a directory or a file
	if (isDirectory(this->fullPath))
	{
		this->handleDirectory();
	}
	else
	{
		this->handleFile();
	}
}

void	Response::handleDirectory()
{
	// check if the requested path has a "/" at the end
	// still not sure about this redirection stuff ???
	if (this->fullPath[this->fullPath.length() - 1] != '/')
	{
		this->statusCode = 301;
		throw std::exception();
	}

	// open the directory and check if it's open
	DIR* dir = opendir(this->fullPath.c_str());
	if (dir == NULL)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	// loop through the directory content and check if it has the index file
	bool hasIndex = false;
	struct dirent* ent;
	while ((ent = readdir(dir)) != NULL)
	{
		if (std::string(ent->d_name) == this->location.getIndex())
		{
			// if the directory has the index file, append it to the full path
			this->fullPath.append(this->location.getIndex());
			hasIndex = true;
			break;
		}
	}

	if (hasIndex)
	{
		this->handleFile();
	}
	else
	{
		this->handleAutoindex(dir);
	}

	closedir(dir);
}

void	Response::handleFile()
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

	// check if the location has CGI
	if (this->location.getCgi() == "off")
	{
		// open the file and check if it's open
		std::ifstream file(this->fullPath.c_str());
		if (!file.is_open())
		{
			this->statusCode = 500;
			throw std::exception();
		}

		// read the file content and store it in the body
		std::stringstream buffer;
		buffer << file.rdbuf();
		this->body = buffer.str();
		file.close();

		// set the status code to 200 and build the response content
		this->statusCode = 200;
		this->buildResponseContent();
	}
	else if (this->location.getCgi() == "on")
	{
		this->handleCGI();
	}
}

void	Response::handleAutoindex(DIR* dir)
{
	// check if the directory has autoindex
	if (this->location.getAutoindex() == "off")
	{
		this->statusCode = 403;
		throw std::exception();
	}
	else if (this->location.getAutoindex() == "on")
	{
		// store the directory content in the body
		struct dirent* ent;
		while ((ent = readdir(dir)) != NULL)
		{
			if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
			{
				this->body.append("<a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a><br>" + "\n");
			}
		}

		// set the status code to 200 and build the response content
		this->statusCode = 200;
		this->buildResponseContent();
	}
}

void	Response::handleCGI()
{
	// TODO: later
}
