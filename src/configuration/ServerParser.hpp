#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webserv.hpp"

class Location;

class ServerParser
{
    private:
        /* data */
        bool                            error;
        std::string                     port;
        std::string                     host;
        std::string                     server_name;
        std::map<short, std::string>    error_pages;
        std::string                     client_max_body_size;
        std::vector<Location *>         _location;
    public:
        //constructors
        ServerParser();
        ~ServerParser();
        ServerParser(const ServerParser& other);
        ServerParser& operator=(const ServerParser& other);

        //getters
        std::string getPort() const;
        const std::string& getHost() const;
        const std::string& getServerName() const;
        const std::map<short, std::string>& getErrorPages() const;
        std::string getClientMaxBodySize() const;
        std::vector<Location*> get_locations() const;
        
        //setters
        void setPort(const std::string& );
        void setHost(const std::string& );
        void setServerName(const std::string& );
        void setErrorPages(short, const std::string& );
        void setClientMaxBodySize(const std::string& );
        void set_locations(Location* new_locations);

        //functions
        void        printTheServerInfo();
        std::vector<ServerParser *> get_server(std::string filename);

        void parse_config_file(std::string filename, ServerParser& config, Location& loc);
        void parse_config_file_help(std::string key, std::string value, ServerParser& config);
        void print_server_elements(Location & loc);
        void fill_vec_location(Location& loc);

        void        clear();
};

#endif