# pragma once

#include "../webserv.hpp"

class Request {
    private :
        std::string                             Path;
        std::string                             Query;
        std::map<std::string, std::string>      requestHeaders;
        std::vector<u_int8_t>                   Body;
        std::string                             bodyString;
        std::string                             Boundary;
        Methods                                 Method;

    public :
        
        Request();

        ~Request();

        void                    clear();

        // ? ----------------------------- getters -----------------------------------

        std::string &                                       getPath();

        std::string  &                                      getQuery();

        const std::map<std::string, std::string>&           getrequestHeaders();

        std::string   &                                     getHeader(std::string );

        std::string &                                       getBody();

        std::string  &                                      getBoundary();

        Methods &                                           getMethods();

        // std::string &                                       getMethodString();


        // ? ----------------------------- setters -----------------------------------

        void        setHeader(std::string &, std::string &);

        void        setBody(std::string body);


        // ? Methods ---------------------------------------------------------------- 

        void        substrRequestBodyString(int );
};