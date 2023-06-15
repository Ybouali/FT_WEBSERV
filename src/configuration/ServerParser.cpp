#include "ServerParser.hpp"

void ServerParser::setPort(const std::string& value) {
    for (size_t i = 0; i < value.size(); i++)
    {
        if (!std::isdigit(value[i]))
            parse_error("port");
    }
    port = value;
}

const std::string& ServerParser::getHost() const {
    return host;
}

void ServerParser::setHost(const std::string& value) {
    
    std::regex ip_regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    
    if (!std::regex_match(value, ip_regex) || value == "0.0.0.0")
        parse_error("host");

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

void ServerParser::setErrorPages(const std::string & codeStatus, const std::string& path) {

    if (codeStatus.empty() || path.empty())
        parse_error("page_error");

    if (codeStatus.length() != 3)
        parse_error("(error page) code status");

    for (size_t i = 0; i < codeStatus.length(); i++)
    {
        if (!isdigit(codeStatus[i]))
            parse_error("(error page) code status");
    }
    short code = std::stoi(codeStatus);
    if (!isValidHTTPStatusCode(code))
        parse_error("(error page) code status");
    
    try
    {
        std::string::size_type pos = path.find_last_of('.');
        if (pos == std::string::npos)
            throw std::exception();
        std::string result = path.substr(pos, path.length());
        if (result != ".html")
            throw std::exception();
    }
    catch(const std::exception)
    {
        parse_error("(error page) code status"); 
    }
    error_pages[code] = path;
}

unsigned long ServerParser::getClientMaxBodySize() const {
    return client_max_body_size;
}

void ServerParser::setClientMaxBodySize(const std::string& value) {
    for (size_t i = 0; i < value.size(); i++)
    {
        if (!std::isdigit(value[i]))
            parse_error("client max body size");
    }

    try
    {
        client_max_body_size = std::stoul(value);
    }
    catch(const std::exception)
    {
        parse_error("client max body size");
    }
    
}

std::vector<Location> ServerParser::get_locations() const {
    return _location;
}

void ServerParser::set_locations(Location new_locations) { _location.push_back(new_locations); }

ServerParser::ServerParser(const ServerParser& other)
{
    *this = other;
}

ServerParser& ServerParser::operator=(const ServerParser& other) {
    if (this != &other) {
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
    this->port.clear();
    this->host.clear();
    this->server_name.clear();
    this->error_pages.clear();
    this->client_max_body_size = 0;
    this->_location.clear();
}

ServerParser::ServerParser()
    : port(), host() , server_name(), error_pages() , client_max_body_size() , _location()
{

}
ServerParser::~ServerParser(){
    this->clear();
}

std::vector<ServerParser> ServerParser::get_server(std::string filename){
    ServerParser        s;
    Location            loc;

    std::string key , value;
    std::vector<ServerParser>    vecServers;


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
            s = ServerParser();
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
                    s.setPort(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "host" && !value.empty())
                {
                    s.setHost(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "server_name" && !value.empty())
                {
                    s.setServerName(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "error_page" && !value.empty())
                {
                    std::istringstream ss(line);
                    std::string name, codeStatus, path;

                    ss >> name >> codeStatus >> path;
                    s.setErrorPages(codeStatus, path);


                    path.clear();
                    key.clear();
                    value.clear();
                }
                else if (key == "client_max_body_size" && !value.empty())
                {
                    s.setClientMaxBodySize(value);
                    key.clear();
                    value.clear();
                }
                else if (key == "}" && value.empty())
                {
                    vecServers.push_back(s);
                    key.clear();
                    value.clear();
                    break;
                }
                else if (key == "location" && !value.empty() && line[line.length() - 1] == '[')
                {
                    loc = Location();
                    loc.setLocation(value);
                    while (std::getline(infile, line))
                    {
                        std::istringstream iss_loc_key_val(line);
                        iss_loc_key_val >> key >> value;
                        if (key == "method" && !value.empty())
                            loc.setMethod(line);
                        else if (key == "root" && !value.empty())
                        {
                            loc.setRoot(value);
                            key.clear();
                            value.clear();
                        }
                        else if (key == "cgi" && !value.empty())
                        {
                            loc.setCgi(value);
                            key.clear();
                            value.clear();
                        }
                        else if (key == "upload" && !value.empty())
                        {
                            loc.setUpload(value);
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "autoindex" && !value.empty())
                        {
                            loc.setAutoindex(value);
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "index" && !value.empty())
                        {
                            loc.setIndex(value);
                            key.clear();
                            value.clear();
                        }   
                        else if (key == "redirection" && !value.empty())
                        {
                            loc.setRedirection(value);
                            key.clear();
                            value.clear();
                        }
                        else if (key == "]")
                        {
                            
                            s.set_locations(loc);
                            
                            break;
                        }
                        else
                        {
                            std::cerr << "ERROR LOC {" << key << "} value {" << value << "}" << std::endl;
                            loc.clear();
                            break;
                        }
                    }
                    key.clear();
                    value.clear();
                }
                else if (!key.empty())
                    parse_error("config file");
            }
            
        }
        else
            parse_error("config file");
    }
    return vecServers;
}

void    ServerParser::printTheServerInfo()
{
    std::cout << "HOST (" << this->getHost() << ") \n";
    std::cout << "PORT (" << this->getPort() << ") \n";
    std::cout << "SERVER NAME (" << this->getServerName() << ")\n";
    if (this->getClientMaxBodySize() == 0)
        std::cout << "THE CLIENT MAX BODY SIZE (" << this->getClientMaxBodySize() << ") \n";
    std::cout << "\n\n";
    for (size_t i = 0; i < this->get_locations().size(); i++)
    {
        std::cout << "|||||||||||||||| START PRINTING LOCATION NUMBER [" << i + 1 <<  "] |||||||||||||||||||" << std::endl;
        this->get_locations()[i].printLocationInfo();
        std::cout << "|||||||||||||||| END PRINTING LOCATION NUMBER [" << i + 1 <<  "] |||||||||||||||||||||" << std::endl;
    }
    std::cout << "\n";   
}