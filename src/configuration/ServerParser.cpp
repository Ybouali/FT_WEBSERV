#include "ServerParser.hpp"

void ServerParser::setPort(const std::string& value) {
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

const std::string& ServerParser::getHost() const {
    return host;
}

void ServerParser::setHost(const std::string& value) {
    std::regex ip_regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if (!std::regex_match(value, ip_regex))
        this->error = true;
    host = value;
}

const std::string& ServerParser::getServerName() const {
    return server_name;
}

void ServerParser::setServerName(const std::string& value) {
    server_name = value;
}

const std::map<short, std::string>& ServerParser::getErrorPages() const {
    return error_pages;
}

std::string ServerParser::getPort() const { return port; }

void ServerParser::setErrorPages(short codeStatus, const std::string& value) {
    error_pages[codeStatus] = value;
}

std::string ServerParser::getClientMaxBodySize() const {
    return client_max_body_size;
}

void ServerParser::setClientMaxBodySize(const std::string& value) {
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

std::vector<Location*> ServerParser::get_locations() const {
    return _location;
}

void ServerParser::set_locations(Location* new_locations) { _location.push_back(new_locations); }

ServerParser::ServerParser(const ServerParser& other)
{
    *this = other;
}

ServerParser& ServerParser::operator=(const ServerParser& other) {
    if (this != &other) {
        error = other.error;
        port = other.port;
        host = other.host;
        server_name = other.server_name;
        error_pages = other.error_pages;
        client_max_body_size = other.client_max_body_size;
        _location = other._location;
        this->error_pages = other.error_pages;
    }
    return *this;
}

void        ServerParser::clear()
{
    this->error = false;
    this->port.clear();
    this->host.clear();
    this->server_name.clear();
    this->error_pages.clear();
    this->client_max_body_size.clear();
    this->_location.clear();
}

ServerParser::ServerParser()
    : error(false), port(), host() , server_name(), error_pages() , client_max_body_size() , _location()
{

}
ServerParser::~ServerParser(){
    this->clear();
}

std::vector<ServerParser*> ServerParser::get_server(std::string filename){
    ServerParser *  s;
    Location *      loc;

    std::string key , value;
    std::vector<ServerParser*>    vecServers;


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
            s = new ServerParser();
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
                    short codeS = 1;
                    std::string errorPath;
                    std::string var;

                    std::istringstream iss(line);
                    while (std::getline(iss, var, ' '))
                    {
                        if (!var.empty() && var != "error_page")
                        {
                            if (var.size() == 3)
                            {
                                for (size_t i = 0; i < var.size(); i++)
                                {
                                    if (!std::isdigit(var[i]))
                                        codeS = 0;
                                }
                                if (codeS)
                                    codeS = std::stoi(var);
                                else
                                    std::cerr << "[WARNING] :: The code status of the page should be a number [" << var << "]" << std::endl;
                            }
                            else
                                errorPath = var;
                            if (!errorPath.empty() && codeS)
                                this->setErrorPages(codeS, errorPath);
                        }
                    }
                    key.clear();
                    value.clear();
                }
                else if (key == "client_max_body_size" && !value.empty())
                {
                    s->setClientMaxBodySize(value);
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
                    loc = new Location();
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
                        else if (key == "cgi" && !value.empty())
                        {
                            loc->setCgi(value);
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

void    ServerParser::printTheServerInfo()
{
    std::cout << "HOST (" << this->getHost() << ") \n";
    std::cout << "PORT (" << this->getPort() << ") \n";
    std::cout << "SERVER NAME (" << this->getServerName() << ")\n";
    // if (!this->getErrorPages().empty())
    //     std::cout << "PATH TO THE ERROR PAGES  ("<< this->getErrorPages() << ") \n";
    if (!this->getClientMaxBodySize().empty())
        std::cout << "THE CLIENT MAX BODY SIZE (" << this->getClientMaxBodySize() << ") \n";
    std::cout << "\n\n";
    for (size_t i = 0; i < this->get_locations().size(); i++)
    {
        std::cout << "|||||||||||||||| START PRINTING LOCATION NUMBER [" << i + 1 <<  "] |||||||||||||||||||" << std::endl;
        this->get_locations()[i]->printLocationInfo();
        std::cout << "|||||||||||||||| END PRINTING LOCATION NUMBER [" << i + 1 <<  "] |||||||||||||||||||||" << std::endl;
    }
    std::cout << "\n";   
}