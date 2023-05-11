#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include "location.hpp"

class server
{
    private:
        /* data */
        int port;
        std::string host;
        std::string server_name;
        std::string error_pages;
        int client_max_body_size;
        std::vector<std::string> cgi_extension;
        std::vector<location> _location;
    public:
        server(/* args */);
        ~server();

        //getters
        int getPort() const;
        const std::string& getHost() const;
        const std::string& getServerName() const;
        const std::string& getErrorPages() const;
        int getClientMaxBodySize() const;
        const std::vector<std::string>& getCgiExtension() const;
        std::vector<location> get_locations() const;
        
        //setters
        void setPort(int value);
        void setHost(const std::string& value);
        void setServerName(const std::string& value);
        void setErrorPages(const std::string& value);
        void setClientMaxBodySize(int value);
        void setCgiExtension(std::vector<std::string>& value);
        void set_locations(std::vector<location> new_locations);

        //functions
        void parse_config_file(std::string filename, server& config, location& loc);
        void parse_config_file_help(std::string key, std::string value, server& config);
        void print_server_elements(void);
};

#endif