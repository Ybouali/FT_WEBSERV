#include "server.hpp"

bool is_digit_port(std::string str) {
    //port range is 0 to 65535.
    if (str.length() < 2 && str.length() > 5) {
        return false;
    }
    for (size_t i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool is_digit_max(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

void server::parse_config_file_help(std::string key, std::string value, server& config)
{
    if (key == "port") {
        if (is_digit_port(value) == true)
            config.setPort(std::stoi(value));
    } else if (key == "host") {
        config.setHost(value);
    } else if (key == "server_name") {
        config.setServerName(value);
    } else if (key == "error_page") {
        config.setErrorPages(value);
    } else if (key == "client_max_body_size") {
        if (is_digit_max(value) == true)
            config.setClientMaxBodySize(std::stoi(value));
    }
}

void server::parse_config_file(std::string filename, server& config, location& loc) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: could not open config file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<std::string> cgis;
        std::string key, value;
        iss >> key >> value;
        config.parse_config_file_help(key, value, config);
        if (key == "cgi") {
            while (iss >> key){
                cgis.push_back(value);
                cgis.push_back(key);
            }
            config.setCgiExtension(cgis);
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
    std::cout << "*******   server elements   *******" << std::endl;
    std::cout << "port == " << port << std::endl;
    std::cout << "host == " << host << std::endl;
    std::cout << "server == " << server_name << std::endl;
    std::cout << "error page == " << error_pages << std::endl;
    std::cout << "max body size == " << client_max_body_size << std::endl;

    std::vector<std::string>::iterator it;
    int i = 1;
    for(it = cgi_extension.begin(); it != cgi_extension.end(); ++it){
        std::cout << "Cgi item == " << i << " == " << *it << std::endl;
        i++;
    }
    std::cout << "***********************************" << std::endl;
}