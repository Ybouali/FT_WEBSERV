#include "../Response.hpp"

void	Response::handlePostMethod()
{
	try
	{
		// check if the location supports upload
		if (!this->location.getUpload().empty())
		{
			// Check if the folder is exist
			DIR *dir = opendir(this->location.getUpload().c_str());
			if (!dir)
			{
				this->statusCode = 404;
				throw std::exception();
			}
			closedir(dir);

			struct stat st;
			const char *filename = this->request.getNameFileBody().c_str();
			stat(filename, &st);
			off_t size = st.st_size;

			// Response :: Entity Too Large
			if ( (unsigned long) size > this->server.getClientMaxBodySize())
			{
				this->statusCode = 404;
				throw std::exception();
			}

			// check if the location has CGI
			if (this->location.getCgi() == "on")
			{
				this->handleCGI();
			}
			else
			{
				this->statusCode = this->request.uploadFile(this->location.getUpload());
				throw std::exception();
			}
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
