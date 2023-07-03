#include "Response.hpp"

void	Response::handleCGI()
{
	// get the file extension
	std::string extension = this->fullPath.substr(this->fullPath.find_last_of(".") + 1);

	// get the cgi file path based on the file extension
	std::string cgiFilePath;
	if (extension == "php")
	{
		cgiFilePath = "./cgi/php-cgi";
	}
	else if (extension == "py")
	{
		cgiFilePath = "./cgi/python-cgi";
	}
	else
	{
		this->statusCode = 502;
		throw std::exception();
	}

	// check if the cgi has execute permission
	if (access(cgiFilePath.c_str(), X_OK) == -1)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	// set the necessary environment variables for the cgi
	std::map<std::string, std::string> env;
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["REDIRECT_STATUS"] = "200";
	env["SERVER_NAME"] = this->request.getHost();
	env["SERVER_PORT"] = std::to_string(this->request.getPort());
	env["PATH_INFO"] = this->request.getPath();
	env["PATH_TRANSLATED"] = this->fullPath;
	env["REQUEST_METHOD"] = this->method;
	env["QUERY_STRING"] = this->request.getQuery();
	env["HTTP_COOKIE"] = this->request.getHeader("Cookie");
	if (this->method == "POST")
	{
		env["CONTENT_TYPE"] = skipWhitespaceBeginAnd(this->request.getHeader("Content-Type"));
		env["CONTENT_LENGTH"] = skipWhitespaceBeginAnd(this->request.getHeader("Content-Length"));
	}

	// create pipe for the cgi output
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	// fork the process to execute the cgi
	pid_t pid = fork();
	if (pid == -1)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	// if the process is the child process
	if (pid == 0)
	{
		// build the argv for the cgi
		char** argv = new char*[3];
		argv[0] = new char[cgiFilePath.length() + 1];
		argv[1] = new char[this->fullPath.length() + 1];
		argv[2] = NULL;
		strcpy(argv[0], cgiFilePath.c_str());
		strcpy(argv[1], this->fullPath.c_str());

		// convert the environment variables to char**
		char** envp = new char*[env.size() + 1];
		int i = 0;
		for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++)
		{
			envp[i] = new char[it->first.length() + it->second.length() + 2];
			strcpy(envp[i], (it->first + "=" + it->second).c_str());
			i++;
		}
		envp[i] = NULL;

		if (this->method == "POST")
		{
			// open the file body fd
			int infd = open(this->request.getNameFileBody().c_str(), O_RDONLY);
			if (infd == -1)
			{
				exit(EXIT_FAILURE);
			}

			// redirect the stdin to the file body fd
			if (dup2(infd, STDIN_FILENO) == -1)
			{
				exit(EXIT_FAILURE);
			}

			close(infd);
		}

		// redirect the stdout to the write end of the pipe
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			exit(EXIT_FAILURE);
		}

		close(pipefd[0]);
		close(pipefd[1]);

		// exucute the cgi
		if (execve(cgiFilePath.c_str(), argv, envp) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}

	// close the pipe write end and set the response fd to the pipe read end
	close(pipefd[1]);
	this->fd = pipefd[0];

	sleep(1);

	// wait for the child process to finish
	int status;
	pid_t ret = waitpid(pid, &status, WNOHANG);
	if (ret > 0)
	{
		// check if the cgi has finished successfully
		if (WEXITSTATUS(status) == EXIT_FAILURE)
		{
			this->statusCode = 500;
			throw std::exception();
		}

		this->readStatus = true;
		this->statusCode = 200;
		this->responseContent = getResponsePage(this->statusCode, false, "");
		this->responseContent.append("Connection: keep-alive");
	}
	else if (ret == -1)
	{
		this->statusCode = 500;
		throw std::exception();
	}
	else if (ret == 0)
	{
		this->statusCode = 504;
		kill(pid, SIGKILL);
		throw std::exception();
	}
}
