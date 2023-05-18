#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webserv.hpp"

class Location;

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
        std::vector<Location *> _location;
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
        std::vector<Location*> get_locations() const;
        
        //setters
        void setPort(const std::string& );
        void setHost(const std::string& );
        void setServerName(const std::string& );
        void setErrorPages(const std::string& );
        void setIndex(const std::string& );
        void setRoot(const std::string& );
        void setClientMaxBodySize(const std::string& );
        void setCgiExtension(std::vector<std::string>& );
        void set_locations(Location* new_locations);

        //functions
        void        printTheServerInfo();
        std::vector<server*> get_server(std::string filename);

        void parse_config_file(std::string filename, server& config, Location& loc);
        void parse_config_file_help(std::string key, std::string value, server& config);
        void print_server_elements(Location & loc);
        void fill_vec_location(Location& loc);

        void        clear();
};

#endif