#include "Location.hpp"

Location::Location()
    : location_path(), method(), root(), upload(), autoindex(), index(), redirection()
{ }

Location::~Location() {
    this->clear();
}

Location::Location(const Location& other)
{
    *this = other;
}

Location& Location::operator=(const Location& other) {
    if (this != &other) {
        this->location_path = other.location_path;
        this->method = other.method;
        this->root = other.root;
        this->upload = other.upload;
        this->autoindex = other.autoindex;
        this->index = other.index;
        this->redirection = other.redirection;
        this->cgi = other.cgi;
    }
    return *this;
}


const std::vector<std::string>& Location::getMethod() const {
    return this->method;
}

void Location::setMethod(std::string & line) {
    std::istringstream iss(line);

    std::string method, method1, method2, method3;
    iss >> method >> method1 >> method2 >> method3;

    if (!method1.empty())
    {
        if (method1 != "GET" && method1 != "POST" && method1 != "DELETE")
            parse_error("method");
        this->method.push_back(method1);
    }
    if (!method2.empty())
    {
        if (method2 != "GET" && method2 != "POST" && method2 != "DELETE")
            parse_error("method");
        this->method.push_back(method2);
    }
    if (!method3.empty())
    {
        if (method3 != "GET" && method3 != "POST" && method3 != "DELETE")
            parse_error("method");
        this->method.push_back(method3);
    }
}

const std::string& Location::getRoot() const {
    return root;
}

void Location::setRoot(const std::string& value) {
    if (value[value.length() - 1] != '/')
        parse_error("root");
    root = value;
}

const std::string& Location::getUpload() const {
    return this->upload;
}

void Location::setUpload(const std::string& value) {
    if (value[value.length() - 1] != '/')
        parse_error("upload");
    this->upload = value;
}

const std::string& Location::getAutoindex() const {
    return this->autoindex;
}

void Location::setAutoindex(const std::string& value) {
    if (value != "on" && value != "off")
        parse_error("autoindex");
    this->autoindex = value;
}

const std::string& Location::getIndex() const { return this->index; }

void Location::setIndex(const std::string& value) {
    this->index = value;
}

void    Location::clear()
{
    this->location_path.clear();
    this->method.clear();
    this->root.clear();
    this->upload.clear();
    this->autoindex.clear();
    this->index.clear();
    this->redirection.clear();
    this->cgi.clear();
}

const std::string& Location::getRedirection() const {
    return this->redirection;
}

void Location::setRedirection(const std::string& value) {
    redirection = value;
}

const std::string& Location::getLocation() const {
    return this->location_path;
}

void Location::setLocation(const std::string& value){
    if (value[0] != '/')
        parse_error("location");
    this->location_path = value;
}

void    Location::setCgi(std::string & value)
{
    if (value != "on" && value != "off")
        parse_error("cgi");
    this->cgi = value;
}

const std::string&    Location::getCgi() const { return this->cgi; }

void Location::printLocationInfo(){
    std::cout << "LOCATION        (" << this->getLocation() << ") \n";

    for (size_t i = 0; i < this->getMethod().size(); i++)
        std::cout << "METHOD          (" << this->getMethod()[i] << ") \n";
    
    std::cout << "ROOT            (" << this->getRoot() << ") \n";
    std::cout << "UPLOAD          (" << this->getUpload() << ")\n";
    std::cout << "AUTOINDEX       (" << this->getAutoindex() << ") \n";
    std::cout << "CGI             (" << this->getCgi() << ") \n";
    std::cout << "INDEX           (" << this->getIndex() << ") \n";
    std::cout << "REDIRECTION     (" << this->getRedirection() << ") \n";
}