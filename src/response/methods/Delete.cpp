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
			this->handleDeleteDirectory();
		}
		else
		{
			// check if the location has CGI
			if (this->location.getCgi() == "on")
			{
				this->handleDeleteCGI();
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
}

void	Response::handleDeleteDirectory()
{
	// check if the directory path ends with a slash
	if (this->fullPath.at(this->fullPath.length() - 1) != '/')
	{
		this->statusCode = 409;
		throw std::exception();
	}

	try
	{
		// check if the location has CGI
		if (this->location.getCgi() == "on")
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

			// if the directory has the index file, handle it using CGI, otherwise return 403
			if (hasIndex)
			{
				this->handleDeleteCGI();
			}
			else
			{
				this->statusCode = 403;
				throw std::exception();
			}
		}
		else
		{
			this->handleDeleteDirectoryContent();
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
					this->handleDeleteDirectory();

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
		}

		closedir(dir);

		// set the full path to the original path to delete the parent directory
		this->fullPath = originalPath;
		this->handleDeleteEmptyDirectory();
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

void	Response::handleDeleteEmptyDirectory()
{
	// check if the empty directory has been deleted successfully
	if (rmdir(this->fullPath.c_str()) == 0)
	{
		this->statusCode = 204;
	}
	else
	{
		// check if the directory has write permission
		if (access(this->fullPath.c_str(), W_OK) == 0)
		{
			this->statusCode = 500;
		}
		else
		{
			this->statusCode = 403;
		}
		throw std::exception();
	}
}

void	Response::handleDeleteFile()
{
	// check if the file has been deleted successfully
	if (remove(this->fullPath.c_str()) == 0)
	{
		this->statusCode = 204;
	}
	else
	{
		this->statusCode = 500;
		throw std::exception();
	}
}

void	Response::handleDeleteCGI()
{
	// TODO: later
}
