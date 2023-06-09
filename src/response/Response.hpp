#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../webserv.hpp"
#include "../request/Request.hpp"

class Response
{
    private:
		ConfigServer	server;
        Request			request;
		std::string		responseContent;
		short			statusCode;
		std::string		body;
		Location		location;
		std::string		method;
		std::string		fullPath;

    public:
		static MimeTypes mime_type;
        Response();
        ~Response();

        void	clear();

		// ----------------------------- Getters -----------------------------------

		const ConfigServer&	getConfigServer() const;
		const Request&		getRequest() const;
		const std::string&	getResponseContent() const;
		short				getStatusCode() const;
		const std::string&	getBody() const;
		const Location&		getLocation() const;
		const std::string&	getMethod() const;
		const std::string&	getFullPath() const;

        // ----------------------------- Setters -----------------------------------

		void	setConfigServer(const ConfigServer&);
		void	setRequest(const Request&);
		void	setResponseContent(const std::string&);
		void	setStatusCode(const int);
		void	setBody(const std::string&);
		void	setLocation(const Location&);
		void	setMethod(const std::string&);
		void	setFullPath(const std::string&);

        // ----------------------------- Methodes -----------------------------------

		void	buildResponse();
		void	buildResponseContent();
		void	isLocationMatched();
		void	isRedirectionExist();
		void	isMethodAllowed();
		void	isResourceExist();

		// Get
		void	handleGetMethod();
		void	handleGetDirectory();
		void	handleGetFile();
		void	handleGetAutoindex(DIR*);
		void	handleGetCGI();

		// Post
		void	handlePostMethod();
		void	handlePostDirectory();
		void	handlePostFile();
		void	handlePostCGI();

		// Delete
		void	handleDeleteMethod();
};

#endif
