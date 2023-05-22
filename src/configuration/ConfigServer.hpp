#pragma once

#include "../webserv.hpp"

class Location;

class ConfigServer {
    private :
        uint16_t						Port;
	    in_addr_t						Host; 
		std::string						serverName;
		std::map<short, std::string>    errorPages;
		unsigned long					clientMaxBodySize;
        std::vector<Location * >        locationList;
        int     						Fd;
        struct sockaddr_in 				serverAddress;

    public : 
        ConfigServer();
        
        ConfigServer(std::string , std::string , std::string , std::vector<Location * > , std::map<short, std::string>);

        ConfigServer(const ConfigServer &);

        ConfigServer & operator=(const ConfigServer &);

        ~ConfigServer();

        void        clear();

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

        void                                setClientMaxBodySize(std::string);

        void                                setIndex(std::string);

        void                                setErrorPages(std::map<short, std::string>);

        void                                setFd(int);

        // ! Methods ----------------------------------------------------------------

        void                                setupServer();
};