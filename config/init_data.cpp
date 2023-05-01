/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zezzine <zezzine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 13:34:23 by zezzine           #+#    #+#             */
/*   Updated: 2023/05/01 16:22:54 by zezzine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

void config::init_data2(char *av){
    config c;
    c.check_file(av);
    std::ifstream configFile(av);
    std::map<std::string, std::string> configMap;

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.empty() || line.find('=') == std::string::npos)
            continue;

        std::istringstream iss(line);
        std::string key, value;
        std::getline(iss, key, '=');
        std::getline(iss, value);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        configMap[key] = value;
    }

    c.fill_attributes(c, configMap);
    c.print_values(c);
}

void config::fill_attributes(config & c, std::map<std::string, std::string> configMap){
    c.port = configMap.at("port");
    c.host = configMap.at("host");
    c.server_name = configMap.at("server_name");
    c.error_pages = configMap.at("error_page");
    c.client_max_body_size = configMap.at("body_size_limit");
    c.cgi_extension = configMap.at("cgi");
    c.location = configMap.at("location");
    c.method = configMap.at("method");
    c.root = configMap.at("root");
    c.upload = configMap.at("upload");
    c.autoindex = configMap.at("autoindex");
    c.default2 = configMap.at("default");
    c.redirect = configMap.at("redirect");
}

// void config::check_attributes(config & c, std::map<std::string, std::string> configMap){
    
// }

void config::print_values(config & c){
    std::cout << c.port << std::endl;
    std::cout << c.host << std::endl;
    std::cout << c.server_name << std::endl;
    std::cout << c.error_pages << std::endl;
    std::cout << c.client_max_body_size << std::endl;
    std::cout << c.cgi_extension << std::endl;
    std::cout << c.location << std::endl;
    std::cout << c.method << std::endl;
    std::cout << c.root << std::endl;
    std::cout << c.upload << std::endl;
    std::cout << c.autoindex << std::endl;
    std::cout << c.default2 << std::endl;
    std::cout << c.redirect << std::endl;
}

config::config(){}
config::~config(){}