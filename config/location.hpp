#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
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
        std::string _default;
        std::string _return;
        
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

        //setters
        void setMethod(std::vector<std::string>& value);
        void setRoot(const std::string& value);
        void setUpload(const std::string& value);
        void setAutoindex(const std::string& value);
        void setDefault(const std::string& value);
        void setReturn(const std::string& value);
        void setLocation(const std::string& value);

        //functions
        void parse_location(location& loc, std::string line);
        void print_location_elements(void);
    };

#endif