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
        bool On_Off;
        std::string _default;
        std::string redirection;
        
    public:
        location(/* args */);
        ~location();

        //getters
        const std::vector<std::string>& getMethod() const;
        const std::string& getRoot() const;
        const std::string& getUpload() const;
        const std::string& getAutoindex() const;
        const std::string& getDefault() const;
        const std::string& getReturn() const;
        const std::string& getLocation() const;
        const bool& getOnOff() const;

        //setters
        void setMethod(std::vector<std::string>& );
        void setRoot(const std::string& );
        void setUpload(const std::string& );
        void setAutoindex(const std::string& );
        void setDefault(const std::string& );
        void setReturn(const std::string& );
        void setLocation(const std::string& );
        void setOnOff(const bool& );

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