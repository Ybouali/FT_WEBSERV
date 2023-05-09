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
        std::map<u_int8_t, std::string>         methodsString;
        state                                   State;
        size_t                                  bodySize;
        size_t                                  maxBodySize;
        size_t                                  errorCode;
        size_t                                  chunkedLength;
        std::string                             Storage;
        std::string                             keyStorage;
        short                                   methodIndex;
        u_int8_t                                verMajor;
        u_int8_t                                verMinor;
        std::string                             serverName;
        bool                                    fieldsDoneFlag;
        bool                                    bodyFlag;
        bool                                    bodyDoneFlag;
        bool                                    completeFlag;
        bool                                    chunkedFlag;
        bool                                    multiformFlag;

        void                                   handleHeaders();

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

        Methods &                                           getMethod();

        std::string                                         getMethodsString();

        short                                               getCodeError();

        std::string &                                       getServerName();

        bool                                                getMultiformFlag();

        state                                               getState();

        // ? ----------------------------- setters -----------------------------------

        void        setHeader(std::string &, std::string &);

        void        setBody(std::string body);

        void        setMethod(Methods &);

        void        setMaxBodySize(size_t);

        void        setCodeError(short );


        // ? Methods ---------------------------------------------------------------- 

        void        substrRequestBodyString(int );

        bool        keepAlive();
        
        void        readBufferFromReq(char *, std::size_t);

        void        printRequest();
};