#pragma once

#include "../webserv.hpp"

class ConfigServer {
    private :
        uint16_t						Port;
	    in_addr_t						Host; 
		std::string						serverName;
		std::string						Root;
		std::string						Index;
		std::map<short, std::string>    errorPages;
		unsigned long					clientMaxBodySize;
        int     						Fd;
        struct sockaddr_in 				serverAddress;

    public : 
        ConfigServer();
        
        ConfigServer(std::string port, std::string host, std::string ServerName, std::string root, unsigned long ClientMaxBodySize, std::string index);

        ConfigServer(const ConfigServer &);

        ConfigServer & operator=(const ConfigServer &);

        ~ConfigServer();

        void        clear();

        // ! INITIALIZATION OF ERROR PAGES
        void                                initErrorPages();

        //! ----------------------------- getters -----------------------------------

        uint16_t                            getPort() const;
        
        in_addr_t                           getHost() const;

        std::string                         getServerName() const;

        std::string                         getRoot() const;

        unsigned long                       getClientMaxBodySize() const;

        std::string                         getIndex() const;

        std::map<short, std::string>        getErrorPages() const;

        int                                 getFd() const;

        //! ----------------------------- setters -----------------------------------

        void                                setPort(std::string);
        
        void                                setHost(std::string);

        void                                setServerName(std::string);

        void                                setRoot(std::string);

        void                                setClientMaxBodySize(unsigned long);

        void                                setIndex(std::string);

        void                                setErrorPages(std::map<short, std::string>);

        void                                setFd(int);

        // ! Methods ----------------------------------------------------------------

        void                                setupServer();
};