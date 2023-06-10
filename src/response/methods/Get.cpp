#include "../Response.hpp"

void	Response::handleGetMethod()
{
	try
	{
		// check if the requested resource exists
		this->isResourceExist();

		// check if the requested resource is a directory or a file
		if (isDirectory(this->fullPath))
		{
			this->handleGetDirectory();
		}
		else
		{
			this->handleGetFile();
		}
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

void	Response::handleGetDirectory()
{
	// check if the directory path ends with a slash
	if (this->fullPath.at(this->fullPath.length() - 1) != '/')
	{
		// if it doesn't, redirect the client to the same path with a slash at the end
		this->statusCode = 301;
		this->fullPath.append("/");
		throw std::exception();
	}

	// open the directory and check if it's open
	DIR* dir = opendir(this->fullPath.c_str());
	if (dir == NULL)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	struct dirent* ent;
	bool hasIndex = false;

	// loop through the directory content and check if it has the index file
	while ((ent = readdir(dir)) != NULL)
	{
		// if the directory has the index file, append it to the full path
		std::string fileName = ent->d_name;
		if (fileName == this->location.getIndex())
		{
			this->fullPath.append(fileName);
			hasIndex = true;
			break;
		}
	}

	closedir(dir);

	try
	{
		// if the directory has the index file, handle it as a file, otherwise check if it has autoindex
		if (hasIndex)
		{
			this->handleGetFile();
		}
		else
		{
			this->handleGetAutoindex(dir);
		}
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

void	Response::handleGetFile()
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
	if (this->location.getCgi() == "on")
	{
		this->handleGetCGI();
	}
	else if (this->location.getCgi() == "off")
	{
		// open the file and check if it's open
		std::ifstream file(this->fullPath.c_str());
		if (!file.is_open())
		{
			this->statusCode = 500;
			throw std::exception();
		}

		// TODO: if the requested file is too large
		// read the file content and store it in the body
		std::stringstream buffer;
		buffer << file.rdbuf();
		this->body = buffer.str();

		file.close();

		// set the status code to 200 and build the response content
		this->statusCode = 200;
		this->buildResponseContent();
	}
}

void	Response::handleGetAutoindex(DIR* dir)
{
	// check if the directory has autoindex
	if (this->location.getAutoindex() == "on")
	{
		// loop through the directory content and append it to the body
		struct dirent* ent;
		while ((ent = readdir(dir)) != NULL)
		{
			std::string fileName = ent->d_name;
			if (fileName != "." && fileName != "..")
			{
				this->body.append(fileName);
				this->body.append("\n");
			}
		}

		// set the status code to 200 and build the response content
		this->statusCode = 200;
		this->fullPath.append("index.html");
		this->buildResponseContent();
	}
	else if (this->location.getAutoindex() == "off")
	{
		this->statusCode = 403;
		throw std::exception();
	}
}

void	Response::handleGetCGI()
{
	// TODO: later
}
