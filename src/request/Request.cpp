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

// ? Public methods ---------------------------------------------------------------- 

void                                            Request::substrRequestBodyString(int bytes) { this->bodyString = this->bodyString.substr(bytes); }

// ? Private methods ---------------------------------------------------------------- 

void                                   Request::handleHeaders()
{
    std::stringstream ss;

    if (this->requestHeaders.count("content-length"))
    {
        this->bodyFlag = true;
        ss << this->requestHeaders["content-length"];
        ss >> this->bodySize;
        std::cout << "content-length :::::::::::::::[" << ss << "]" << std::endl;
    }
    if (this->requestHeaders.count("transfer-encoding"))
    {
        if (this->requestHeaders["transfer-encoding"].find_first_of("chunked") != std::string::npos)
        {
            this->chunkedFlag = true;
            std::cout << "chunked ::::::::::::::::::[" << this->chunkedFlag << "]" << std::endl;
        }
        this->bodyFlag = true;
        std::cout << "transfer-encoding ::::::::::::[" << (this->bodyFlag ? "body existe" : "body is not existe") << "]" << std::endl;
    }
    if (this->requestHeaders.count("host"))
    {
        size_t position = this->requestHeaders["host"].find_first_of(':');
        this->serverName = this->requestHeaders["host"].substr(0, position);
        std::cout << "server name ::::::::::::::::::[" << (this->serverName.empty() ? "there is no server name" : this->serverName) << std::endl;
    }
    if (this->requestHeaders.count("content-type") && this->requestHeaders["content-type"].find("multipart/form-data") != std::string::npos)
    {
        size_t position = this->requestHeaders["content-type"].find("boundary", 0);
        if (position == std::string::npos)
            this->Boundary = this->requestHeaders["content-type"].substr(position + 9, this->requestHeaders["content-type"].size());
        this->multiformFlag = true;
    }



































    
    // if (_request_headers.count("content-type") && _request_headers["content-type"].find("multipart/form-data") != std::string::npos)
    // {
    //     size_t pos = _request_headers["content-type"].find("boundary=", 0);
    //     if (pos != std::string::npos)
    //         this->_boundary = _request_headers["content-type"].substr(pos + 9, _request_headers["content-type"].size());
    //     this->_multiform_flag = true;
    // }
}