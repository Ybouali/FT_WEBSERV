#include "location.hpp"

const std::vector<std::string>& location::getMethod() const {
    return method;
}

void location::setMethod(std::vector<std::string>& value) {
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
    autoindex = value;
}

const std::string& location::getDefault() const {
    return _default;
}

void location::setDefault(const std::string& value) {
    _default = value;
}

const std::string& location::getReturn() const {
    return _return;
}

void location::setReturn(const std::string& value) {
    _return = value;
}

const std::string& location::getLocation() const {
    return location_path;
}

void location::setLocation(const std::string& value){
    location_path = value;
}

location::location(){}
location::~location(){}