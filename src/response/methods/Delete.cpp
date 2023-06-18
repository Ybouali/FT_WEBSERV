#include "../Response.hpp"

void	Response::handleDeleteMethod()
{
	try
	{
		// check if the requested resource exists
		this->isResourceExist();

		// check if the requested resource is a directory or a file
		if (isDirectory(this->fullPath))
		{
			// check if the directory path ends with a slash
			if (this->fullPath.at(this->fullPath.length() - 1) != '/')
			{
				this->statusCode = 409;
				throw std::exception();
			}
			else
			{
				// check if the location has CGI
				if (this->location.getCgi() == "on")
				{
					this->handleDeleteDirectoryCGI();
				}
				else
				{
					this->handleDeleteDirectoryContent();
				}
			}
		}
		else
		{
			// check if the location has CGI
			if (this->location.getCgi() == "on")
			{
				this->handleCGI();
			}
			else
			{
				this->handleDeleteFile();
			}
		}
	}
	catch (const std::exception& e)
	{
		throw;
	}

	this->connectionStatus = true;
	this->statusCode = 204;

	std::string errorPage = this->server.getErrorPages().find(this->statusCode)->second;
	this->responseContent = getResponsePage(this->statusCode, true, errorPage);

	// std::cout << this->responseContent << std::endl;
}

void	Response::handleDeleteDirectoryCGI()
{
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
		// if the directory has the index file, handle it using CGI, otherwise return 403
		if (hasIndex)
		{
			this->handleCGI();
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

void	Response::handleDeleteDirectoryContent()
{
	// save the original path
	std::string originalPath = this->fullPath;

	// open the directory and check if it's open
	DIR* dir = opendir(this->fullPath.c_str());
	if (dir == NULL)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	try
	{
		struct dirent* ent;

		// loop through the directory content and delete subdirectories and files
		while ((ent = readdir(dir)) != NULL)
		{
			std::string fileName = ent->d_name;
			if (fileName != "." && fileName != "..")
			{
				// check if the entry is a directory or a file
				if (ent->d_type == DT_DIR)
				{
					// append the file name to the full path
					this->fullPath.append(fileName + "/");

					// recursively delete the subdirectory content
					this->handleDeleteDirectoryContent();

					// delete the empty subdirectory
					this->handleDeleteEmptyDirectory();
				}
				else
				{
					// append the file name to the full path
					this->fullPath.append(fileName);

					// delete the file
					this->handleDeleteFile();
				}
			}

			// restore the original path
			this->fullPath = originalPath;
		}

		closedir(dir);
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

void	Response::handleDeleteEmptyDirectory()
{
	// check if the directory has write and read permission
	if (access(this->fullPath.c_str(), W_OK | R_OK) == 0)
	{
		// check if the directory has been deleted successfully
		if (rmdir(this->fullPath.c_str()) == 0)
		{
			this->statusCode = 204;
		}
		else
		{
			this->statusCode = 500;
			throw std::exception();
		}
	}
	else
	{
		this->statusCode = 403;
		throw std::exception();
	}
}

void	Response::handleDeleteFile()
{
	// check if the file has write permission
	if (access(this->fullPath.c_str(), W_OK) == 0)
	{
		// check if the file has been deleted successfully
		if (unlink(this->fullPath.c_str()) == 0)
		{
			this->statusCode = 204;
		}
		else
		{
			this->statusCode = 500;
			throw std::exception();
		}
	}
	else
	{
		this->statusCode = 403;
		throw std::exception();
	}
}
