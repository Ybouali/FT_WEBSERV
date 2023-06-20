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
	env["SCRIPT_NAME"] = cgiFilePath;
	env["REQUEST_METHOD"] = this->method;
	if (this->method == "GET" && !this->request.getQuery().empty())
		env["QUERY_STRING"] = this->request.getQuery();
	env["CONTENT_TYPE"] = getContentType(this->fullPath);
	if (this->method == "POST")
		env["CONTENT_LENGTH"] = this->request.getHeader("Content-Length");

	// // print environment variables
	// for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++)
	// {
	// 	std::cout << it->first << ": " << it->second << std::endl;
	// }

	// // create the output file for the cgi
	// std::string outputFilePath = "./cgi/" + generateRandomFileName() + ".html";
	// int outputFileFd = open(outputFilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);

	// // check if the output file is open
	// if (outputFileFd == -1)
	// {
	// 	this->statusCode = 500;
	// 	throw std::exception();
	// }

	// create pipe for the cgi
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	// fork the process to execute the cgi
	pid_t pid = fork();

	// check if the fork is successful
	if (pid == -1)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	// if the process is the child process
	if (pid == 0)
	{
		// redirect the output to the pipe
		dup2(pipefd[1], STDOUT_FILENO);

		// build the argv for the cgi
		char** argv = new char*[3];
		argv[0] = new char[cgiFilePath.length() + 1];
		strcpy(argv[0], cgiFilePath.c_str());
		argv[1] = new char[this->fullPath.length() + 1];
		strcpy(argv[1], this->fullPath.c_str());
		argv[2] = NULL;

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

		// exucute the cgi
		if (execve(cgiFilePath.c_str(), argv, envp) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}

	// wait for the child process to finish
	int status;
	waitpid(pid, &status, O_NONBLOCK);

	// check if the cgi has finished successfully
	if (WEXITSTATUS(status) == EXIT_FAILURE)
	{
		this->statusCode = 500;
		throw std::exception();
	}

	close(pipefd[1]);

	// read the output from the pipe
	char buffer[BUF_SIZE];
	int readBytes;

	while ((readBytes = read(pipefd[0], buffer, BUF_SIZE)) > 0)
	{
		this->body.append(buffer, readBytes);
	}

	close(pipefd[0]);

	this->statusCode = 200;

	this->responseContent = getResponsePage(this->statusCode, false, "");
	this->responseContent.append("Content-Type: text/html\r\n");
	this->responseContent.append("Content-Length: ");
	this->responseContent.append(std::to_string(this->body.length()));
	this->responseContent.append("\r\n\r\n");
	this->responseContent.append(this->body);

	this->connectionStatus = true;

	// this->fd = pipefd[0];
	// this->readStatus = true;
	// this->statusCode = 200;

	// this->handleGetFile();
}
