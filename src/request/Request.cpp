# include "Request.hpp"

Request::Request() 
    : Path(), Query(), requestHeaders(), Body(), bodyString(), Boundary()
{ }

Request::~Request()
{ 
    this->clear();
}

void            Request::clear()
{
    this->Path.clear();
    this->Query.clear();
    this->requestHeaders.clear();
    this->Body.clear();
    this->bodyString.clear();
    this->Boundary.clear();
    
}

// ? ----------------------------- getters -----------------------------------

std::string &                                       Request::getPath() { return this->Path; }

std::string  &                                      Request::getQuery() { return this->Query; }

const std::map<std::string, std::string>&           Request::getrequestHeaders() { return this->requestHeaders; }

std::string &                                       Request::getHeader(std::string key) { return this->requestHeaders[key]; }

std::string &                                       Request::getBody() { return this->bodyString; }

std::string &                                       Request::getBoundary() { return this->Boundary; }

Methods &                                           Request::getMethods() { return this->Method; }

// ? ----------------------------- setters -----------------------------------

void                                            Request::setHeader(std::string & key, std::string & value)
{
    key = skipWhitespaceBeginAnd(key);
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    this->requestHeaders[key] = value;
}

void                                            Request::setBody(std::string body)
{
    this->Body.clear();
    this->Body.insert(this->Body.begin(), body.begin(), body.end());
    this->bodyString = body;
}

// ? Methods ---------------------------------------------------------------- 

void                                            Request::substrRequestBodyString(int bytes) { this->bodyString = this->bodyString.substr(bytes); }
