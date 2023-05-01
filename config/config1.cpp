/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config1.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zezzine <zezzine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:41:31 by zezzine           #+#    #+#             */
/*   Updated: 2023/05/01 14:23:51 by zezzine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

// int main(int ac, char **av) {

//     if (ac == 2){
//         std::string file = av[1];
//         std::ifstream infile(file);
        
//         if (!infile) {
//             std::cerr << "Error opening file\n";
//             exit(1);
//         }

//         std::string line;
//         bool in_server_block = false;
//         int listen_port = 0;
//         std::string server_name = "";
//         std::string root_dir = "";
//         std::string cgi_extension = "";
//         std::string index_page = "";
//         std::string upload_dir = "";
//         int client_max_body_size = 0;

//         while (std::getline(infile, line)) {
//         // Remove comments from the line
//         // std::string::size_type comment_pos = line.find('#');
//         // if (comment_pos != std::string::npos) {
//         //     line.erase(comment_pos);
//         // }

//         // Trim leading and trailing whitespaces
//         line.erase(0, line.find_first_not_of(" \t"));
//         line.erase(line.find_last_not_of(" \t") + 1);
//         std::cout << line << std::endl;
//         // Check for start of server block
//         if (line == "server {") {
//             in_server_block = true;
//             continue;
//         }

//         // Check for end of server block
//         if (line == "}") {
//             if (in_server_block) {
//             // Output the parsed values for this server block
//             std::cout << "Listen Port: " << listen_port << std::endl;
//             std::cout << "Server Name: " << server_name << std::endl;
//             std::cout << "Root Directory: " << root_dir << std::endl;
//             std::cout << "CGI Extension: " << cgi_extension << std::endl;
//             std::cout << "Index Page: " << index_page << std::endl;
//             std::cout << "Upload Directory: " << upload_dir << std::endl;
//             std::cout << "Client Max Body Size: " << client_max_body_size << std::endl;
//             std::cout << std::endl;
            
//             // Reset the parsed values for the next server block
//             listen_port = 0;
//             server_name = "";
//             root_dir = "";
//             cgi_extension = "";
//             index_page = "";
//             upload_dir = "";
//             client_max_body_size = 0;
//             in_server_block = false;
//             }
//             continue;
//         }

//         // Parse the key-value pairs
//         // std::string key = line.substr(0, line.find_first_of(" \t"));
//         // std::string value = line.substr(line.find_first_of(" \t") + 1);
//         // if (in_server_block) {
//         //     if (key == "listen") {
//         //     listen_port = std::stoi(value);
//         //     } else if (key == "server_name") {
//         //     server_name = value;
//         //     } else if (key == "root") {
//         //     root_dir = value;
//         //     } else if (key == "cgi") {
//         //     cgi_extension = value;
//         //     } else if (key == "index") {
//         //     index_page = value;
//         //     } else if (key == "upload") {
//         //     upload_dir = value;
//         //     } else if (key == "client_max_body_size") {
//         //     client_max_body_size = std::stoi(value);
//         //     }
//         // }
//         }

//         infile.close();
//     }    
//     return 0;
// }



// int main (int ac, char **av){
//     if (ac == 2){
//         config c;

//         c.init_data(av[1]);
//         // c.remvoe_all_spaces(av[1]);
        
//     }

// }

// #include <iostream>
// #include <fstream>
// #include <string>
// #include <algorithm>

// int main(int ac, char **av) {
//     if (ac == 2){
//         std::ifstream input_file(av[1]);
//         if (!input_file.is_open()) {
//             std::cerr << "Failed to open input file" << std::endl;
//             return 1;
//         }

//         std::string line;
//         while (std::getline(input_file, line)) {
//             // Remove all whitespace characters from the line:
//             line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
//             line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

//             // Output the line to the standard output:
//             std::cout << line << std::endl;
//         }

//         input_file.close();    
//     }
//   return 0;
// }

// #include <iostream>
// #include <fstream>
// #include <map>
// #include <string>

// int main(int ac, char **av) {
//     std::ifstream file(av[1]);
//     std::string line;
//     (void)ac;
//     std::map<std::string, std::string> settings;

//     while (std::getline(file, line)) {
//         if (line.empty() || line[0] == '#') {
//             continue;
//         }

//         std::string::size_type equal_pos = line.find('=');
//         if (equal_pos == std::string::npos) {
//             continue;
//         }

//         std::string key = line.substr(0, equal_pos);
//         std::string value = line.substr(equal_pos + 1);

//         // Remove leading and trailing whitespace from the key and value
//         std::string::iterator key_begin = key.begin();
//         std::string::iterator key_end = key.end();
//         while (key_begin != key_end && std::isspace(*key_begin)) {
//             ++key_begin;
//         }
//         while (key_begin != key_end && std::isspace(*(key_end - 1))) {
//             --key_end;
//         }

//         std::string::iterator value_begin = value.begin();
//         std::string::iterator value_end = value.end();
//         while (value_begin != value_end && std::isspace(*value_begin)) {
//             ++value_begin;
//         }
//         while (value_begin != value_end && std::isspace(*(value_end - 1))) {
//             --value_end;
//         }

//         // Insert the key-value pair into the map
//         settings.insert(std::make_pair(std::string(key_begin, key_end), std::string(value_begin, value_end)));
//     }

//     // Print out the map
//     for (std::map<std::string, std::string>::iterator it = settings.begin(); it != settings.end(); ++it) {
//         std::cout << it->first << " = " << it->second << std::endl;
//     }

//     return 0;
// }

#include "config.hpp"

int main(int ac, char **av) {
    (void)ac;
    config c;
    
    c.init_data2(av[1]);
    return 0;
}
