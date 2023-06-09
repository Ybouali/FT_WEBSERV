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
			else if (this->location.getCgi() == "off")
			{
				this->handleDeleteFile();
			}
		}
	}
	catch (const std::exception& e)
	{
		throw;
	}
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
		else if (this->location.getCgi() == "off")
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
				std::string filePath = this->fullPath + "/" + fileName;

				// check if the entry is a directory or a file
				if (ent->d_type == DT_DIR)
				{
					// recursively delete the directory content
					this->fullPath = filePath;
					this->handleDeleteDirectory();

					// delete the empty subdirectory
					this->handleDeleteEmptyDirectory();
				}
				else
				{
					// delete the file
					this->fullPath = filePath;
					this->handleDeleteFile();
				}
			}
		}

		closedir(dir);

		// delete the empty directory
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
	// delete the empty directory
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
	// delete the file
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
