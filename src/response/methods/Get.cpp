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
			// check if the requested file has read permission
			if (access(this->fullPath.c_str(), R_OK) == -1)
			{
				this->statusCode = 403;
				throw std::exception();
			}

			// check if the location has CGI
			if (this->location.getCgi() == "on")
			{
				this->handleCGI();
			}
			else
			{
				this->handleGetFile();
			}
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
		// if the directory has the index file, handle it as a file, otherwise check if it has autoindex
		if (hasIndex)
		{
			// check if the requested file has read permission
			if (access(this->fullPath.c_str(), R_OK) == -1)
			{
				this->statusCode = 403;
				throw std::exception();
			}

			// check if the location has CGI
			if (this->location.getCgi() == "on")
			{
				this->handleCGI();
			}
			else
			{
				this->handleGetFile();
			}
		}
		else
		{
			this->handleGetAutoindex();
		}
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

void	Response::handleGetFile()
{
	char buffer[BUF_SIZE];

	if (!this->readStatus)
	{
		// open the requested file and check if it's open
		if ((this->fd = open(this->fullPath.c_str(), O_RDONLY)) == -1)
		{
			this->statusCode = 500;
			throw std::exception();
		}

		// set the response headers
		this->statusCode = 200;
		this->buildResponseContent();

		// set the read status to true to read the file content
		this->readStatus = true;
	}
	else
	{
		// clear the response content that was sent in the previous loop
		this->responseContent.clear();

		// read the file content by BUF_SIZE bytes
		if ((readBytes = read(this->fd, buffer, BUF_SIZE)) == -1)
		{
			this->statusCode = 500;
			throw std::exception();
		}

		if (readBytes > 0)
		{
			this->responseContent = std::string(buffer, readBytes);
		}
		else
		{
			this->connectionStatus = true;
			close(this->fd);
		}
	}
}

void	Response::handleGetAutoindex()
{
	// check if the directory has autoindex
	if (this->location.getAutoindex() == "on")
	{
		// open the directory and check if it's open
		DIR* dir = opendir(this->fullPath.c_str());
		if (dir == NULL)
		{
			this->statusCode = 500;
			throw std::exception();
		}

		this->body.append("<html><head><title>Index of ");
		this->body.append(this->request.getPath());
		this->body.append("</title></head><body><h1>Index of ");
		this->body.append(this->request.getPath());
		this->body.append("</h1><hr><pre>");
		this->body.append("<style> body { background-color: #f0f0f0; font-family: sans-serif;} </style>");

		// loop through the directory content and append it to the body
		struct dirent* ent;
		while ((ent = readdir(dir)) != NULL)
		{
			std::string fileName = ent->d_name;
			if (fileName != "." && fileName != "..")
			{
				this->body.append("<a href=\"");
				this->body.append(fileName);
				this->body.append("\">");
				this->body.append(fileName);
				this->body.append("</a><br>");
			}
		}

		this->body.append("</pre><hr></body></html>");

		closedir(dir);

		// build the response content
		this->connectionStatus = true;
		this->statusCode = 200;
		this->fullPath.append("index.html");
		this->buildResponseContent();
	}
	else
	{
		this->statusCode = 403;
		throw std::exception();
	}
}
