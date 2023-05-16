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
        bool        error;
        std::string port;
        std::string host;
        std::string server_name;
        std::string error_pages;
        std::string root;
        std::string index;
        std::string client_max_body_size;
        std::vector<std::string> cgi_extension;
        std::vector<location *> _location;
    public:
        //constructors
        server();
        ~server();
        server(const server& other);
        server& operator=(const server& other);

        //getters
        std::string getPort() const;
        const std::string& getHost() const;
        const std::string& getServerName() const;
        const std::string& getErrorPages() const;
        const std::string& getRoot() const;
        const std::string& getIndex() const;
        std::string getClientMaxBodySize() const;
        const std::vector<std::string>& getCgiExtension() const;
        std::vector<location*> get_locations() const;
        
        //setters
        void setPort(const std::string& );
        void setHost(const std::string& );
        void setServerName(const std::string& );
        void setErrorPages(const std::string& );
        void setIndex(const std::string& );
        void setRoot(const std::string& );
        void setClientMaxBodySize(const std::string& );
        void setCgiExtension(std::vector<std::string>& );
        void set_locations(location* new_locations);

        //functions
        std::vector<server*> get_server(std::string filename);

        void parse_config_file(std::string filename, server& config, location& loc);
        void parse_config_file_help(std::string key, std::string value, server& config);
        void print_server_elements(location & loc);
        void fill_vec_location(location& loc);

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