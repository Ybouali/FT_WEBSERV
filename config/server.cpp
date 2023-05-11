#include "server.hpp"

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

server::server(){}
server::~server(){}