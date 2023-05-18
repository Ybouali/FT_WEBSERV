#include "Location.hpp"

const std::vector<std::string>& Location::getMethod() const {
    return method;
}

void Location::setMethod(std::vector<std::string>& value) {
    value.erase(value.begin());
    method = value;
}

const std::string& Location::getRoot() const {
    return root;
}

void Location::setRoot(const std::string& value) {
    root = value;
}

const std::string& Location::getUpload() const {
    return upload;
}

void Location::setUpload(const std::string& value) {
    upload = value;
}

const std::string& Location::getAutoindex() const {
    return autoindex;
}

void Location::setAutoindex(const std::string& value) {
    if (value == "on" || value == "off"){
        autoindex = value;
    }
    else
        error = true;
}

const std::string& Location::getIndex() const {
    return index;
}

void Location::setIndex(const std::string& value) {
    index = value;
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
    return redirection;
}

void Location::setRedirection(const std::string& value) {
    redirection = value;
}

const std::string& Location::getLocation() const {
    return location_path;
}

void Location::setLocation(const std::string& value){
    location_path = value;
}

Location::Location(const Location& other)
{
    *this = other;
}

bool  Location::getError() const { return this->error; }

Location& Location::operator=(const Location& other) {
    if (this != &other) {
        error = other.error;
        location_path = other.location_path;
        method = other.method;
        root = other.root;
        upload = other.upload;
        autoindex = other.autoindex;
        index = other.index;
        redirection = other.redirection;
    }
    return *this;
}

void    Location::setCgi(std::string & value)
{
    if (value == "on" || value == "off"){
        cgi = value;
    }
    else
        error = true;
}

std::string    Location::getCgi() const { return this->cgi; }

Location::Location()
    : error(false) , location_path(), method(), root(), upload(), autoindex(), index(), redirection()
{

}

Location::~Location(){
    this->clear();
}

void Location::printLocationInfo(){
    std::cout << "LOCATION        (" << this->getLocation() << ") \n";

    std::cout << "METHOD          (" << this->getMethod()[0] << ") \n";
    std::cout << "METHOD          (" << this->getMethod()[1] << ") \n";
    std::cout << "METHOD          (" << this->getMethod()[2] << ") \n";
    
    std::cout << "ROOT            (" << this->getRoot() << ") \n";
    std::cout << "UPLOAD          (" << this->getUpload() << ")\n";
    std::cout << "AUTOINDEX       (" << this->getAutoindex() << ") \n";
    std::cout << "CGI             (" << this->getCgi() << ") \n";
    std::cout << "INDEX           (" << this->getIndex() << ") \n";
    std::cout << "REDIRECTION     (" << this->getRedirection() << ") \n";
}