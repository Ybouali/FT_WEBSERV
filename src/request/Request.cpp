# include "Request.hpp"

Request::Request() 
    : Path(), Query(), requestHeaders(), Body(), bodyString()
{ }

Request::~Request()
{ }

void            Request::clear()
{
    this->Path.clear();
    this->Query.clear();
    this->requestHeaders.clear();
    this->Body.clear();
    this->bodyString.clear();
}

// ? ----------------------------- getters -----------------------------------

std::string                                     Request::getPath() const { return this->Path; }

std::string                                     Request::getQuery() const { return this->Query; }

const std::map<std::string, std::string>&       Request::getrequestHeaders() const { return this->requestHeaders; }

std::string                                     Request::getHeader(std::string key) { return this->requestHeaders[key]; }

std::vector<u_int8_t>                           Request::getBody() const { return this->Body; }

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
