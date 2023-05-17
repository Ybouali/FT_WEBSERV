#include "location.hpp"

const std::vector<std::string>& location::getMethod() const {
    return method;
}

void location::setMethod(std::vector<std::string>& value) {
    value.erase(value.begin());
    method = value;
}

const std::string& location::getRoot() const {
    return root;
}

void location::setRoot(const std::string& value) {
    root = value;
}

const std::string& location::getUpload() const {
    return upload;
}

void location::setUpload(const std::string& value) {
    upload = value;
}

const std::string& location::getAutoindex() const {
    return autoindex;
}

void location::setAutoindex(const std::string& value) {
    if (value == "on" || value == "off"){
        autoindex = value;
    }
    else
        error = true;
}

const std::string& location::getIndex() const {
    return index;
}

void location::setIndex(const std::string& value) {
    index = value;
}

void    location::clear()
{
    this->location_path.clear();
    this->method.clear();
    this->root.clear();
    this->upload.clear();
    this->autoindex.clear();
    this->index.clear();
    this->redirection.clear();
}

const std::string& location::getRedirection() const {
    return redirection;
}

void location::setRedirection(const std::string& value) {
    redirection = value;
}

const std::string& location::getLocation() const {
    return location_path;
}

void location::setLocation(const std::string& value){
    location_path = value;
}

location::location(const location& other)
{
    *this = other;
}

bool  location::getError() const { return this->error; }

location& location::operator=(const location& other) {
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

location::location()
    : error(false) , location_path(), method(), root(), upload(), autoindex(), index(), redirection()
{

}

location::~location(){
    this->clear();
}

void location::printLocationInfo(){
    std::cout << "LOCATION        (" << this->getLocation() << ") \n";

    std::cout << "METHOD          (" << this->getMethod()[0] << ") \n";
    std::cout << "METHOD          (" << this->getMethod()[1] << ") \n";
    std::cout << "METHOD          (" << this->getMethod()[2] << ") \n";
    
    std::cout << "ROOT            (" << this->getRoot() << ") \n";
    std::cout << "UPLOAD          (" << this->getUpload() << ")\n";
    std::cout << "AUTOINDEX       (" << this->getAutoindex() << ") \n";
    std::cout << "INDEX           (" << this->getIndex() << ") \n";
    std::cout << "REDIRECTION     (" << this->getRedirection() << ") \n";
}