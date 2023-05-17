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

void        server::clear()
{
    this->error = false;
    this->port.clear();
    this->host.clear();
    this->server_name.clear();
    this->error_pages.clear();
    this->root.clear();
    this->index.clear();
    this->client_max_body_size.clear();
    this->cgi_extension.clear();
    this->_location.clear();
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
                    s->setPort(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "host" && !value.empty())
                {
                    s->setHost(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "server_name" && !value.empty())
                {
                    s->setServerName(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "error_page" && !value.empty())
                {
                    s->setErrorPages(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "client_max_body_size" && !value.empty())
                {
                    s->setClientMaxBodySize(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "index" && !value.empty())
                {
                    s->setIndex(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "root" && !value.empty())
                {
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
                            vecCgi.push_back(token);
                    }
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
                    loc = new location();
                    loc->setLocation(value);
                    while (std::getline(infile, line))
                    {
                        std::istringstream iss_loc_key_val(line);
                        iss_loc_key_val >> key >> value;
                        if (key == "method" && !value.empty())
                        {
                            std::istringstream iss_p10(line);
                            std::string token;
                            std::vector<std::string> vecMethod;
                            while (std::getline(iss_p10, token, ' ')) {
                                if (!token.empty())
                                {
                                    vecMethod.push_back(token);
                                }
                            }
                            loc->setMethod(vecMethod);
                        }
                        else if (key == "root" && !value.empty())
                        {
                            loc->setRoot(value);
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "upload" && !value.empty())
                        {
                            loc->setUpload(value);
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "autoindex" && !value.empty())
                        {
                            loc->setAutoindex(value);
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "index" && !value.empty())
                        {
                            loc->setIndex(value);
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "redirection" && !value.empty())
                        {
                            loc->setRedirection(value);
                            key.clear();
                            value.clear();
                        }
                        else if (key == "]")
                        {
                            if (!loc->getError())
                                s->set_locations(loc);
                            else 
                            {
                                loc->clear();
                                delete loc;
                            }
                            break;
                        }
                        else
                        {
                            std::cerr << "ERROR LOC {" << key << "} value {" << value << "}" << std::endl;
                            loc->clear();
                            delete loc;
                            break;
                        }
                    }
                    key.clear();
                    value.clear();
                }
                else if (!key.empty())
                {
                    std::cerr << "ERROR SER {" << key << "} value {" << value << "}" << std::endl;
                    s->clear();
                    delete s;
                    break;
                }
            }
            
        }
    }
    return vecServers;
}

void    server::printTheServerInfo()
{
    std::cout << "HOST (" << this->getHost() << ") \n";
    std::cout << "PORT (" << this->getPort() << ") \n";
    std::cout << "SERVER NAME (" << this->getServerName() << ")\n";
    std::cout << "THE PATH OF THE ROOT IS  (" << this->getRoot() << ") \n";
    std::cout << "THE INDEX PAGE IS        (" << this->getIndex() << ") \n";
    if (!this->getErrorPages().empty())
        std::cout << "PATH TO THE ERROR PAGES  ("<< this->getErrorPages() << ") \n";
    if (!this->getClientMaxBodySize().empty())
        std::cout << "THE CLIENT MAX BODY SIZE (" << this->getClientMaxBodySize() << ") \n";
    if (!this->getCgiExtension().empty())
    {
        std::cout << "THE CGI EXE IS           (" << this->getCgiExtension()[0] << ") \n";
        std::cout << "THE CGI PATH IS          (" << this->getCgiExtension()[1] << ")";
    }
    std::cout << "\n\n";
    for (size_t i = 0; i < this->get_locations().size(); i++)
    {
        std::cout << "|||||||||||||||| START PRINTING LOCATION NUMBER [" << i + 1 <<  "] |||||||||||||||||||" << std::endl;
        this->get_locations()[i]->printLocationInfo();
        std::cout << "|||||||||||||||| END PRINTING LOCATION NUMBER [" << i + 1 <<  "] |||||||||||||||||||||" << std::endl;
    }
    std::cout << "\n";   
}