#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../webserv.hpp"

class Response
{
    private:
		ConfigServer		server;
        Request				request;
		std::string			responseContent;
		const std::string	protocol;
		std::string			statusCode;
		std::string			statusMessage;
		std::string			contentType;
		std::string			contentLength;
		const std::string	serverName;
		std::string			Date;

    public:
        Response();
        ~Response();

        void	clear();

		// ----------------------------- Getters -----------------------------------

		const ConfigServer	getConfigServer() const;
		const Request		getRequest() const;
		const std::string&	getResponseContent() const;
		const std::string&	getProtocol() const;
		const std::string&	getStatusCode() const;
		const std::string&	getStatusMessage() const;
		const std::string&	getContentType() const;
		const std::string&	getContentLength() const;
		const std::string&	getServerName() const;
		const std::string&	getDate() const;

        // ----------------------------- Setters -----------------------------------

		void	setConfigServer(const ConfigServer&);
		void	setRequest(const Request&);
		void	setResponseContent(const std::string&);
		void	setStatusCode(const std::string&);
		void	setStatusMessage(const std::string&);
		void	setContentType(const std::string&);
		void	setContentLength(const std::string&);
		void	setDate(const std::string&);

        // ----------------------------- Methodes -----------------------------------

		// void				getdatetime();
		void	createResponse();
};

#endif
