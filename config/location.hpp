#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <exception>
#include <algorithm>

class location
{
    private:
        /* data */
        std::string location_path;
        std::vector<std::string> method;
        std::string root;
        std::string upload;
        std::string autoindex;
        std::string index;
        std::string redirection;
        
    public:
        location(/* args */);
        ~location();
        location (const location& other);
        location& operator=(const location& other);

        //getters
        const std::vector<std::string>& getMethod() const;
        const std::string& getRoot() const;
        const std::string& getUpload() const;
        const std::string& getAutoindex() const;
        const std::string& getIndex() const;
        const std::string& getReturn() const;
        const std::string& getLocation() const;

        //setters
        void setMethod(std::vector<std::string>& );
        void setRoot(const std::string& );
        void setUpload(const std::string& );
        void setAutoindex(const std::string& );
        void setIndex(const std::string& );
        void setReturn(const std::string& );
        void setLocation(const std::string& );

        //functions
        void parse_location(location& loc, std::string line);
        void print_location_elements(void);
        bool check_config_file(const std::string& filename, char c1, char c2);
        //execption
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