# pragma once

#include "../webserv.hpp"

class Request {
    private :
        std::string                             Path;
        std::string                             Query;
        std::map<std::string, std::string>      requestHeaders;
        std::vector<u_int8_t>                   Body;
        std::string                             bodyString;

    public :
        
        Request();

        ~Request();

        void                    clear();

        // ? ----------------------------- getters -----------------------------------

        std::string                                     getPath() const;

        std::string                                     getQuery() const;

        const std::map<std::string, std::string>&       getrequestHeaders() const;

        std::string                                     getHeader(std::string );

        std::vector<u_int8_t>                           getBody() const;


        // ? ----------------------------- setters -----------------------------------

        void        setHeader(std::string &, std::string &);

        void        setBody(std::string body);


        // ? Methods ---------------------------------------------------------------- 

        void        substrRequestBodyString(int );
};