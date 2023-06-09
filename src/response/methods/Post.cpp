#include "../Response.hpp"

void	Response::handlePostMethod()
{
	// check if the location supports upload
	if (!this->location.getUpload().empty())
	{
		this->request.uploadFile(this->location.getUpload());
	}
	else
	{
		try
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
		catch (const std::exception& e)
		{
			throw;
		}
	}
}

void	Response::handlePostDirectory()
{
	// check if the directory path ends with a slash
	if (this->fullPath[this->fullPath.length() - 1] != '/')
	{
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
		if (std::string(ent->d_name) == this->location.getIndex())
		{
			this->fullPath.append(this->location.getIndex());
			hasIndex = true;
			break;
		}
	}

	try
	{
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

	closedir(dir);
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
	else if (this->location.getCgi() == "off")
	{
		this->statusCode = 403;
		throw std::exception();
	}
}

void	Response::handlePostCGI()
{
	// TODO: later
}
