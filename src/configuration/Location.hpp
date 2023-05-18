#ifndef LOCATION_HPP
#define LOCATION_HPP
 
#include "../webserv.hpp"

class Location
{
    private:
        /* data */
        bool        error;
        std::string location_path;
        std::vector<std::string> method;
        std::string root;
        std::string upload;
        std::string autoindex;
        std::string index;
        std::string redirection;
        std::string cgi;
        
    public:
        Location();
        ~Location();
        Location (const Location& other);
        Location& operator=(const Location& other);

        //getters
        const std::vector<std::string>& getMethod() const;
        const std::string& getRoot() const;
        const std::string& getUpload() const;
        const std::string& getAutoindex() const;
        const std::string& getIndex() const;
        const std::string& getRedirection() const;
        const std::string& getLocation() const;
        bool  getError() const;
        std::string getCgi() const;

        //setters
        void setMethod(std::vector<std::string>& );
        void setRoot(const std::string& );
        void setUpload(const std::string& );
        void setAutoindex(const std::string& );
        void setIndex(const std::string& );
        void setRedirection(const std::string& );
        void setLocation(const std::string& );
        void setCgi(std::string & );

        void    clear();

        void printLocationInfo();
    };

#endif