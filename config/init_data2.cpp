/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data2.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zezzine <zezzine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 14:24:31 by zezzine           #+#    #+#             */
/*   Updated: 2023/05/01 16:23:16 by zezzine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

void config::check_file(char *av){
    std::string file = av;
    std::ifstream infile(file);

    if (!infile){
        std::cerr << "Error opening file\n";
        exit(1);
    }
}

int theres_spaces(std::string line)
{
    std::size_t pos = line.find("="); 
    std::string key = line.substr(0, pos); 
    std::string value = line.substr(pos + 1);
    std::size_t key_last = key.find_last_not_of(" \t"); 
    std::size_t value_first = value.find_first_not_of(" \t"); 
    bool has_space_before = key_last != std::string::npos && key_last != key.length() - 1;
    bool has_space_after = value_first != std::string::npos && value_first != 0;
    // std::cout << "Has space before: " << has_space_before << std::endl;
    // std::cout << "Has space after: " << has_space_after << std::endl;
    if (has_space_after == 0 || has_space_before == 0)
        return 1;
    return 0;
}

void config::init_data(char *av){
    config c;
    std::string line;
    check_file(av);
    std::ifstream infile(av);
    bool in_server_block = false;
    
    c.port = "";
    c.host = "";
    c.server_name = "";
    c.error_pages = "";
    c.client_max_body_size = "";
    c.cgi_extension = "";
    
    if (std::getline(infile, line)) {
        // Remove all spaces from the first line:
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        // Output the modified first line:
        in_server_block = true;
    }

    while (std::getline(infile, line)){
        //erase the spaces that are in config file.
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (in_server_block) {
            std::istringstream iss(line);
            std::string word;
            std::string key = line.substr(0, line.find_first_of(" \t"));
            std::string value = line.substr(line.find_first_of(" \t") + 1);
            if (theres_spaces(line) == 1)
                std::cerr << "Error close to equal" << std::endl;
            std::cout << line << std::endl;
            if (key == "port"){
                value.erase(0, 1);
                c.port = std::stoi(value);
                // std::cout <<  c.port << std::endl;
            }
            else if (key == "host"){
                value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                value.erase(0, 1);
                c.host = value;
                // std::cout <<  c.host << std::endl;
            }
            else if (key == "server_name"){
                value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                value.erase(0, 1);
                c.server_name = value;
                // std::cout <<  c.server_name << std::endl;
            }
            else if (key == "error_page"){
                value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                value.erase(0, 1);
                c.error_pages = value;
                // std::cout <<  c.error_pages << std::endl;
            }
            else if (key == "body_size_limit"){
                value.erase(0, 1);
                c.client_max_body_size = std::stoi(value);
                // std::cout <<  c.client_max_body_size << std::endl;
            }
            else if (key == "cgi"){
                value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                value.erase(0, 1);
                c.cgi_extension = value;
                // std::cout <<  c.cgi_extension << std::endl;
            }
        }
    }
}

int config::remvoe_all_spaces(char *av){
    std::ifstream input_file(av);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        // Remove all whitespace characters from the line:
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

        // Output the line to the standard output:
        std::cout << line << std::endl;
    }

    input_file.close();    
    return 0;
}
