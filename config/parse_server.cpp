#include "server.hpp"

bool is_ip_valid(std::string ip_address){
    std::regex ip_regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if (std::regex_match(ip_address, ip_regex)) {
        return true;
    }
    return false;
}

bool is_digit_port(std::string str) {
    //port range is 0 to 65535.
    if (str.empty())
        return false;
    for (size_t i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    if (str.length() > 5) {
        return false;
    }
    return true;
}

bool is_digit_max(std::string str) {
    if (str.empty())
        return false;
    for (size_t i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

static int j;

void server::parse_config_file_help(std::string key, std::string value, server& config)
{
    if (key == "port" && config.getPort().empty()) {
        if (is_digit_port(value) == true)
            config.setPort(value), j++;
        else
            throw MyException("Error port value not valid !");
    } else if (key == "host" && config.getHost().empty()) {
        if (value.empty())
            throw MyException("Error host empty !");
        else if (!is_ip_valid(value))
            throw MyException("Error host ip not valid !");
        else
            config.setHost(value), j++;
    } else if (key == "server_name" && config.getServerName().empty()) {
        if (value.empty())
            throw MyException("Error server name empty !");  
        else          
            config.setServerName(value), j++;
    } else if (key == "error_page" && config.getErrorPages().empty()) {
        if (value.empty())
            throw MyException("Error Error_Page empty !");
        else
            config.setErrorPages(value), j++;
    } else if (key == "client_max_body_size" && config.getClientMaxBodySize().empty()) {
        if (is_digit_max(value) == true)
            config.setClientMaxBodySize(value), j++;
        else
            throw MyException("Error Client Max Body Size not valid !");
    } else if (key == "root" && config.getRoot().empty()){
        if (value.empty())
            throw MyException("Error root value empty !");
        else
            config.setRoot(value), j++;
    } else if (key == "index" && config.getIndex().empty()){
        if (value.empty())
            throw MyException("Error index value empty !");
        else
            config.setIndex(value), j++;
    }
}

void server::parse_config_file(std::string filename, server& config, location& loc) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: could not open config file " << filename << std::endl;
        return;
    }
    if (!loc.check_config_file(filename, '[', ']') || !loc.check_config_file(filename, '{', '}'))
        throw MyException("Config File Not Valid !");

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<std::string> cgis;
        std::string key, value;
        iss >> key >> value;
        config.parse_config_file_help(key, value, config);
        if (key == "cgi") {
            int i = 0;
            while (iss >> key){
                cgis.push_back(value);
                cgis.push_back(key);
                i++;
            }
            if (i != 1)
                throw MyException("Error Cgi empty !");
            config.setCgiExtension(cgis), j++;
        }
    }

    std::ifstream infile2(filename);
    std::string line2;
    while (std::getline(infile2, line)){
        std::istringstream iss2(line);
        std::string key, value;
        iss2 >> key >> value;
        loc.parse_location(loc, line);
    }
}

void server::print_server_elements(){
    if (j != 8)
        throw MyException("The number of server Elements Not Valid !");
    std::cout << "*******   server elements   *******" << std::endl;
    std::cout << "port == " << port << std::endl;
    std::cout << "host == " << host << std::endl;
    std::cout << "server == " << server_name << std::endl;
    std::cout << "error page == " << error_pages << std::endl;
    std::cout << "max body size == " << client_max_body_size << std::endl;
    std::cout << "root == " << root << std::endl;
    std::cout << "index == " << index << std::endl;
    
    std::vector<std::string>::iterator it;
    int i = 1;
    for(it = cgi_extension.begin(); it != cgi_extension.end(); ++it){
        std::cout << "Cgi item == " << i << " == " << *it << std::endl;
        i++;
    }
    std::cout << "***********************************" << std::endl;
}