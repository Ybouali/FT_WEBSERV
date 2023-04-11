# include "Request.hpp"

Request::Request() 
    : Path(),
      Query(),
      requestHeaders(),
      Body(),
      bodyString(),
      Boundary(),
      Method(),
      methodsString(),
      State(Request_Line),
      maxBodySize(0),
      bodySize(0),
      errorCode(0),
      chunkedLength(0x0),
      Storage(),
      keyStorage(),
      methodIndex(1),
      verMajor(0),
      serverName(),
      fieldsDoneFlag(false),
      bodyFlag(false),
      bodyDoneFlag(false),
      completeFlag(false),
      chunkedFlag(false),
      multiformFlag(false)
{
    this->methodsString[::GET] = "GET";
    this->methodsString[::POST] = "POST";
    this->methodsString[::POST] = "DELETE";
}

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
    this->methodsString.clear();
    this->State = Request_Line;
    this->maxBodySize = 0;
    this->bodySize = 0;
    this->errorCode = 0;
    this->chunkedLength = 0x0;
    this->Storage.clear();
    this->keyStorage.clear();
    this->methodIndex = 0;
    this->verMajor = 0;
    this->verMinor = 0;
    this->serverName.clear();
    this->fieldsDoneFlag = false;
    this->bodyFlag = false;
    this->bodyDoneFlag = false;
    this->completeFlag = false;
    this->chunkedFlag = false;
    this->multiformFlag = false;
}

// ? ----------------------------- getters -----------------------------------

std::string &                                       Request::getPath() { return this->Path; }

std::string  &                                      Request::getQuery() { return this->Query; }

const std::map<std::string, std::string>&           Request::getrequestHeaders() { return this->requestHeaders; }

std::string &                                       Request::getHeader(std::string key) { return this->requestHeaders[key]; }

std::string &                                       Request::getBody() { return this->bodyString; }

std::string &                                       Request::getBoundary() { return this->Boundary; }

Methods &                                           Request::getMethod() { return this->Method; }

std::string                                         Request::getMethodsString() { return this->methodsString[this->Method]; }

short                                               Request::getCodeError() { return this->errorCode; }

std::string &                                       Request::getServerName() { return this->serverName; }

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

void                                            Request::setMethod(Methods & method) { this->Method = method; }

void                                            Request::setMaxBodySize(size_t size) { this->maxBodySize = size; }

// ? Methods ---------------------------------------------------------------- 

void                                            Request::substrRequestBodyString(int bytes) { this->bodyString = this->bodyString.substr(bytes); }
