#include "server.hpp"


const std::string& server::getRoot() const{
    return root;
}

void server::setRoot(const std::string& value){
    root = value;
}

const std::string& server::getIndex() const{
    return index;
}

void server::setIndex(const std::string& value){
    index = value;
}

std::string server::getPort() const {
    return port;
}

void server::setPort(const std::string& value) {
    port = value;
}

const std::string& server::getHost() const {
    return host;
}

void server::setHost(const std::string& value) {
    host = value;
}

const std::string& server::getServerName() const {
    return server_name;
}

void server::setServerName(const std::string& value) {
    server_name = value;
}

const std::string& server::getErrorPages() const {
    return error_pages;
}

void server::setErrorPages(const std::string& value) {
    error_pages = value;
}

std::string server::getClientMaxBodySize() const {
    return client_max_body_size;
}

void server::setClientMaxBodySize(const std::string& value) {
    client_max_body_size = value;
}

const std::vector<std::string>& server::getCgiExtension() const {
    return cgi_extension;
}

void server::setCgiExtension(std::vector<std::string>& value) {
    cgi_extension = value;
}

std::vector<location> server::get_locations() const {
    return _location;
}

void server::set_locations(std::vector<location> new_locations) {
    _location = new_locations;
}

server::server(const server& other): port(other.port), host(other.host)
    , server_name(other.server_name), error_pages(other.error_pages)
    , root(other.root), index(other.index), client_max_body_size(other.client_max_body_size)
    , cgi_extension(other.cgi_extension), _location(other._location)
{}

server& server::operator=(const server& other) {
    if (this != &other) {
        port = other.port;
        host = other.host;
        server_name = other.server_name;
        error_pages = other.error_pages;
        root = other.root;
        index = other.index;
        client_max_body_size = other.client_max_body_size;
        cgi_extension = other.cgi_extension;
        _location = other._location;
    }
    return *this;
}

server::server(){}
server::~server(){}

#include <cstring>

server server::get_server(std::string filename){
    server s;
    std::string key , value;

    std::string line;
    std::ifstream infile(filename);
    if (!infile.is_open()) 
        std::cerr << "Error : could not open config file " << std::endl;

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        iss >> key >> value;
        if (key == "server" && value == "{")
        {
            while (std::getline(infile, line))
            {
                std::istringstream iss_loc(line);
                iss_loc >> key >> value;
                if (key == "port" && !value.empty())
                {
                    std::cout << "Prot                  {" << value << "}" << std::endl;
                    key.clear();
                    value.clear();
                }
                else if (key == "host" && !value.empty())
                {
                    std::cout << "Host                  {" << value << "}" << std::endl;
                    key.clear();
                    value.clear();
                }
                else if (key == "server_name" && !value.empty())
                {
                    std::cout << "Server_name           {" << value << "}" << std::endl;
                    key.clear();
                    value.clear();
                }
                else if (key == "error_page" && !value.empty())
                {
                    std::cout << "Error_page           {" << value << "}" << std::endl;
                    key.clear();
                    value.clear();
                }
                else if (key == "client_max_body_size" && !value.empty())
                {
                    std::cout << "Client_max_body_size  {" << value << "}" << std::endl;
                    key.clear();
                    value.clear();
                }
                else if (key == "index" && !value.empty())
                {
                    std::cout << "Index                 {" << value << "}" << std::endl;
                    key.clear();
                    value.clear();
                }
                else if (key == "root" && !value.empty())
                {
                    std::cout << "Root                  {" << value << "}" << std::endl;
                    key.clear();
                    value.clear();
                }
                else if (key == "}" && value.empty())
                {
                    key.clear();
                    value.clear();
                    break;
                }
                else if (key == "location" && !value.empty() && line[line.length() - 1] == '[')
                {
                    std::cout << "|||||||||||||||||||||| Location              [" << value << "] ||||||||||||||||||||||" << std::endl;
                    while (std::getline(infile, line))
                    {
                        std::istringstream iss_loc_key_val(line);
                        iss_loc_key_val >> key >> value;
                        if (key == "method" && !value.empty())
                        {
                            std::istringstream iss_p10(line);
                            std::string token;
                            while (std::getline(iss_p10, token, ' ')) {
                                if (!token.empty())
                                    std::cout << "[" << token << "]" << std::endl;
                            }
                        }
                        else if (key == "root" && !value.empty())
                        {
                            std::cout << "Root                  {" << value << "}" << std::endl;
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "upload" && !value.empty())
                        {
                            std::cout << "Upload               {" << value << "}" << std::endl;
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "autoindex" && !value.empty())
                        {
                            std::cout << "Autoindex               {" << value << "}" << std::endl;
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "index" && !value.empty())
                        {
                            std::cout << "Index               {" << value << "}" << std::endl;
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "redirection" && !value.empty())
                        {
                            std::cout << "Redirection               {" << value << "}" << std::endl;
                            key.clear();
                            value.clear();
                        }
                        else if (key == "]")
                            break;
                        else
                        {
                            std::cerr << "ERROR LOC {" << key << "} value {" << value << "}" << std::endl;
                            exit (1);
                        }
                        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!! KEY [" << key << "] VAL [" << value << "]" << std::endl;
                    }
                    key.clear();
                    value.clear();
                    std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
                }
                else if (!key.empty())
                {
                    std::cerr << "ERROR SER {" << key << "} value {" << value << "}" << std::endl;
                    exit(1);
                }
            }
            // Here need to push the server ?? 
        }
    }






    // std::getline(infile, line);
    // std::istringstream iss(line);
    // std::string key, value;
    // iss >> key >> value;
    // if (key == "server" && value == "{")
    // {
    //     while (std::getline(infile, line)){
    //         std::istringstream iss(line);
    //         std::string key, value;
    //         iss >> key >> value;
    //         std::cout << key << std::endl;
    //         if (key == "}")
    //             exit(1);
    //     }

    // }
    return s;
}