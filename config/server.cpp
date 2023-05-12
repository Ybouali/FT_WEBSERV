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