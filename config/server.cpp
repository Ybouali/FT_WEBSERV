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
    for (size_t i = 0; i < value.size(); i++)
    {
        if (!std::isdigit(value[i]))
        {
            std::cerr << "[ERROR] :: the port must be a number [" << value << "]" << std::endl;
            this->error = true;
            exit (1);
        }
    }
    port = value;
}

const std::string& server::getHost() const {
    return host;
}

void server::setHost(const std::string& value) {
    std::regex ip_regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if (!std::regex_match(value, ip_regex))
        this->error = true;
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
    for (size_t i = 0; i < value.size(); i++)
    {
        if (!std::isdigit(value[i]))
        {
            std::cerr << "[ERROR] :: the client max body size must be a number [" << value << "]" << std::endl;
            this->error = true;
        }
    }
    client_max_body_size = value;
}

const std::vector<std::string>& server::getCgiExtension() const {
    return cgi_extension;
}

void server::setCgiExtension(std::vector<std::string>& value) {
    for (size_t i = 1; i < value.size(); i++)
        this->cgi_extension.push_back(value[i]);
}

std::vector<location*> server::get_locations() const {
    return _location;
}

void server::set_locations(location* new_locations) {
    _location.push_back(new_locations);
}

server::server(const server& other)
{
    *this = other;
}

server& server::operator=(const server& other) {
    if (this != &other) {
        error = other.error;
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

server::server()
    : error(false), port(), host() , server_name(), error_pages() , root(), index(), client_max_body_size() , cgi_extension(), _location()
{

}
server::~server(){}

// Create a clear function for the server class ?
#include <cstring>

std::vector<server*> server::get_server(std::string filename){
    server * s;
    location *loc;

    std::string key , value;
    std::vector<server*>    vecServers;


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
            // we shoud create a new server 
            s = new server();
            while (std::getline(infile, line))
            {
                std::istringstream iss_loc(line);
                iss_loc >> key >> value;
                if (key == "#" || key[0] == '#')
                {
                    key.clear();
                    value.clear();
                }  
                else if (key == "port" && !value.empty())
                {
                    std::cout << "Port                  {" << value << "}" << std::endl;
                    s->setPort(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "host" && !value.empty())
                {
                    std::cout << "Host                  {" << value << "}" << std::endl;
                    s->setHost(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "server_name" && !value.empty())
                {
                    std::cout << "Server_name           {" << value << "}" << std::endl;
                    s->setServerName(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "error_page" && !value.empty())
                {
                    std::cout << "Error_page           {" << value << "}" << std::endl;
                    s->setErrorPages(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "client_max_body_size" && !value.empty())
                {
                    std::cout << "Client_max_body_size  {" << value << "}" << std::endl;
                    s->setClientMaxBodySize(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "index" && !value.empty())
                {
                    std::cout << "Index                 {" << value << "}" << std::endl;
                    s->setIndex(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "root" && !value.empty())
                {
                    std::cout << "Root                  {" << value << "}" << std::endl;
                    s->setRoot(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "cgi" && !value.empty())
                {
                    std::istringstream iss_cgi(line);
                    std::string token;
                    std::vector<std::string> vecCgi;
                    while (std::getline(iss_cgi, token, ' ')) {
                        if (!token.empty())
                        {
                            vecCgi.push_back(token);
                            std::cout << "[" << token << "] ";
                        }
                    }
                    std::cout << "\n";
                    s->setCgiExtension(vecCgi);
                    vecCgi.clear();
                    key.clear();
                    value.clear();
                }
                else if (key == "}" && value.empty())
                {
                    if (!s->error)
                        vecServers.push_back(s);
                    key.clear();
                    value.clear();
                    break;
                }
                else if (key == "location" && !value.empty() && line[line.length() - 1] == '[')
                {
                    std::cout << "|||||||||||||||||||||| Location              [" << value << "] ||||||||||||||||||||||" << std::endl;
                    loc = new location();
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
                                    std::cout << "[" << token << "] ";
                            }
                            std::cout << "\n";
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
                        {
                            if (!loc->getError())
                                s->set_locations(loc);
                            break;
                        }
                        else
                        {
                            std::cerr << "ERROR LOC {" << key << "} value {" << value << "}" << std::endl;
                            exit (1);
                        }
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
    return vecServers;
}