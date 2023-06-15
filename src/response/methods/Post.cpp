#include "../Response.hpp"

void	Response::handlePostMethod()
{
	try
	{
		// check if the location supports upload
		if (!this->location.getUpload().empty())
		{
			this->request.uploadFile(this->location.getUpload());
		}
		else
		{
			// check if the requested resource exists
			this->isResourceExist();

			// check if the requested resource is a directory or a file
			if (isDirectory(this->fullPath))
			{
				this->handlePostDirectory();
			}
			else
			{
				this->handlePostFile();
			}
		}
	}
	catch (const std::exception& e)
	{
		throw;
	}

	this->connectionStatus = true;
}

void	Response::handlePostDirectory()
{
	// check if the directory path ends with a slash
	if (this->fullPath.at(this->fullPath.length() - 1) != '/')
	{
		// if it doesn't, redirect the client to the same requested path with a slash at the end
		this->statusCode = 301;
		this->fullPath.replace(0, this->location.getRoot().length(), this->location.getLocation());
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
		// if the directory has the index file, handle it as a file, otherwise return 403
		if (hasIndex)
		{
			this->handlePostFile();
		}
		else
		{
			this->statusCode = 403;
			throw std::exception();
		}
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

void	Response::handlePostFile()
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
		this->handlePostCGI();
	}
	else
	{
		this->statusCode = 403;
		throw std::exception();
	}
}

void	Response::handlePostCGI()
{
	// TODO: later
}
