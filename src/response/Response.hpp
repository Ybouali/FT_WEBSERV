#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../webserv.hpp"

class Response
{
    private:
        std::string     response_content;
        ConfigServer    server;
        Request         request;

        void Date();
    public:
        Response();
        ~Response();

        void            clear();

        void            setRequest(Request req);
        void            setConfigServer(ConfigServer ser);

        Request             getRequest() const;
        ConfigServer        getConfigServer() const;

        // void getdatetime();
};
#endif