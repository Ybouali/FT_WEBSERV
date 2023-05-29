#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../webserv.hpp"

# include "../request/Request.hpp"

class Response
{
    private:
		ConfigServer	server;
        Request			request;
		std::string		responseContent;
		short			statusCode;
		std::string		statusMessage;
		std::string		date;
		std::string		contentType;
		std::string		contentLength;
		std::string		body;
		Location		location;
		std::string		method;

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
		const std::string&	getStatusMessage() const;
		const std::string&	getDate() const;
		const std::string&	getContentType() const;
		const std::string&	getContentLength() const;
		const std::string&	getBody() const;
		const Location&		getLocation() const;
		const std::string&	getMethod() const;

        // ----------------------------- Setters -----------------------------------

		void	setConfigServer(const ConfigServer&);
		void	setRequest(const Request&);
		void	setResponseContent(const std::string&);
		void	setStatusCode(const int);
		void	setStatusMessage(const std::string&);
		void	setDate(const std::string&);
		void	setContentType(const std::string&);
		void	setContentLength(const std::string&);
		void	setBody(const std::string&);
		void	setLocation(const Location&);
		void	setMethod(const std::string&);

        // ----------------------------- Methodes -----------------------------------

		void	buildResponse();
		void	isLocationMatched();
		void	isRedirectionExist();
		void	isMethodAllowed();
};

#endif
