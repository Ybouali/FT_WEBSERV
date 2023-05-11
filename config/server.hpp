#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <exception>
#include <algorithm>
#include <regex>
#include "location.hpp"

class server
{
    private:
        /* data */
        std::string port;
        std::string host;
        std::string server_name;
        std::string error_pages;
        std::string client_max_body_size;
        std::vector<std::string> cgi_extension;
        std::vector<location> _location;
    public:
        server(/* args */);
        ~server();

        //getters
        std::string getPort() const;
        const std::string& getHost() const;
        const std::string& getServerName() const;
        const std::string& getErrorPages() const;
        std::string getClientMaxBodySize() const;
        const std::vector<std::string>& getCgiExtension() const;
        std::vector<location> get_locations() const;
        
        //setters
        void setPort(const std::string& );
        void setHost(const std::string& );
        void setServerName(const std::string& );
        void setErrorPages(const std::string& );
        void setClientMaxBodySize(const std::string& );
        void setCgiExtension(std::vector<std::string>& );
        void set_locations(std::vector<location> new_locations);

        //functions
        void parse_config_file(std::string filename, server& config, location& loc);
        void parse_config_file_help(std::string key, std::string value, server& config);
        void print_server_elements(void);

        //exception
        class MyException : public std::exception {
            public:
                MyException(const std::string& msg) : message(msg) {}
                virtual ~MyException() throw() {}
                virtual const char* what() const throw() {
                    return message.c_str();
                }
            private:
                std::string message;
        };
};

#endif