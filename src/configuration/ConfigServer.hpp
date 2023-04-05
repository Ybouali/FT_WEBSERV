#pragma once

#include "../webserv.hpp"

class ConfigServer {
    private :
        uint16_t						Port;
	    in_addr_t						Host; 
		std::string						serverName;
		std::string						Root;
		unsigned long					clientMaxBodySize;
		std::string						Index;
		bool							autoIndex;
		std::map<short, std::string>    errorPages;
        int     						listenFd;
        struct sockaddr_in 				serverAddress;

		// std::vector<??> 			    locations;
    public : 
        ConfigServer(uint16_t port, std::string host, std::string ServerName, std::string root, unsigned long ClientMaxBodySize, std::string index, bool AutoIndex);

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

        bool                                getAutoIndex() const;

        std::map<short, std::string>        getErrorPages() const;

        int                                 getListenFd() const;

        //! ----------------------------- setters -----------------------------------

        void                                setPort(uint16_t);
        
        void                                setHost(std::string);

        void                                setServerName(std::string);

        void                                setRoot(std::string);

        void                                setClientMaxBodySize(unsigned long);

        void                                setIndex(std::string);

        void                                setAutoIndex(bool);

        void                                setErrorPages(std::map<short, std::string>);

        void                                setListenFd();
};