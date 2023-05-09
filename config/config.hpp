#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <map>

class config
{
    private:
        /* data */
        std::string port;
        std::string host;
        std::string server_name;
        std::string error_pages;
        std::string client_max_body_size;
        std::string cgi_extension;

        std::string location;
        
        std::string method;
        std::string root;
        std::string upload;
        std::string autoindex;
        std::string default2;
        std::string redirect;
        
    public:
        config(/* args */);
        ~config();
        void init_data(char *av);
        void check_file(char *av);
        int remvoe_all_spaces(char *av);
        void init_data2(char *av);
        void print_values(config & c);
        void fill_server_attributes(config & c, std::map<std::string, std::string> configMap);
        bool isNumber(const std::string & str);
        void check_int_attributes(config & c, std::map<std::string, std::string> configMap);
        bool isBraceFound(char *av, char c1, char c2);
        int check_number_server_element(std::string keys[6]);
        int check_number_location_element(std::string keys[6]);
};

#endif 