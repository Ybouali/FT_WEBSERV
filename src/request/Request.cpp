# include "Request.hpp"

MimeTypes Request::mime;

Request::Request() 
    : Path(),
      Query(),
      requestHeaders(),
      Body(),
      Method(),
      methodsString(),
      State(Request_Line),
      bodySize(0),
      maxBodySize(0),
      errorCode(0),
      chunkedLength(0x0),
      Storage(),
      keyStorage(),
      methodIndex(1),
      verMajor(0),
      Host(),
      Port(),
      fdFileBody(0),
      nameFileBody(),
      bodyFlag(false),
      chunkedFlag(false)
{
    this->methodsString[::GET] = "GET";
    this->methodsString[::POST] = "POST";
    this->methodsString[::DELETE] = "DELETE";
}

Request::Request(const Request & other)
{
    *this = other;
}

Request & Request::operator= (const Request & other)
{
    if (this != &other)
    {
        this->Path = other.Path;
        this->Query = other.Query;
        this->requestHeaders = other.requestHeaders;
        this->Body = other.Body;
        this->methodsString = other.methodsString;
        this->State = other.State;
        this->Method = other.Method;
        this->maxBodySize = other.maxBodySize;
        this->bodySize = other.bodySize;
        this->errorCode = other.errorCode;
        this->chunkedLength = other.chunkedLength;
        this->Storage = other.Storage;
        this->keyStorage = other.keyStorage;
        this->methodIndex = other.methodIndex;
        this->fdFileBody = other.fdFileBody;
        this->nameFileBody = other.nameFileBody;
        this->verMajor = other.verMajor;
        this->verMinor = other.verMinor;
        this->Host = other.Host;
        this->Port = other.Port;
        this->bodyFlag = other.bodyFlag;
        this->chunkedFlag = other.chunkedFlag;
    }   
    return *this;
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
    this->methodsString.clear();
    this->State = Request_Line;
    this->maxBodySize = 0;
    this->bodySize = 0;
    this->errorCode = 0;
    this->chunkedLength = 0;
    this->Storage.clear();
    this->keyStorage.clear();
    this->methodIndex = 0;
    this->verMajor = 0;
    this->verMinor = 0;
    this->Host.clear();
    if (this->getFdFileBody() > 0)
        close(this->fdFileBody);
    this->nameFileBody.clear();
    this->Port = 0;
    this->bodyFlag = false;
    this->chunkedFlag = false;
}

// ? ----------------------------- getters -----------------------------------

const std::string &                             Request::getPath() const { return this->Path; }

const std::string &                             Request::getQuery() const { return this->Query; }

const std::map<std::string, std::string>&       Request::getrequestHeaders() const { return this->requestHeaders; }

const std::string &                             Request::getHeader(std::string key) { return this->requestHeaders[key]; }

Methods                                         Request::getMethod() const { return this->Method; }

const std::string&                              Request::getMethodsString() { return this->methodsString[this->Method]; }

short                                           Request::getCodeError() const { return this->errorCode; }

const std::string &                             Request::getHost() const { return this->Host; }

state                                           Request::getState() const { return this->State; }

uint16_t                                        Request::getPort() const { return this->Port; }

int                                             Request::getFdFileBody() const { return this->fdFileBody; }

const std::string &                             Request::getNameFileBody() const { return this->nameFileBody; }



// ? ----------------------------- setters -----------------------------------

void                                            Request::setHeader(std::string & key, std::string & value)
{
    key = skipWhitespaceBeginAnd(key);
    this->requestHeaders[key] = value;
}

void                                            Request::setMethod(Methods & method) { this->Method = method; }

void                                            Request::setMaxBodySize(size_t size) { this->maxBodySize = size; }

void                                            Request::setCodeError(short code) { this->errorCode = code; }

bool                                            Request::openFile()
{
    this->nameFileBody = "/tmp/" + generateRandomFileName();

    if (this->requestHeaders.count("Content-Type"))
        this->nameFileBody += "." + mime.getExeFile(skipWhitespaceBeginAnd(this->requestHeaders["Content-Type"]));
    
    this->fdFileBody = open(this->nameFileBody.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (this->fdFileBody < 0)
    {
        this->setCodeError(500);
        return false;
    }
    return true;
}


bool                                            Request::handleHeaders()
{
    std::stringstream ss;
    
    if (this->getMethodsString() == "POST" && !this->requestHeaders.count("Content-Length") && !this->requestHeaders.count("Transfer-Encoding"))
    {
        this->setCodeError(411);
        return false;
    }
    if (this->getMethodsString() == "POST" && this->requestHeaders.count("Content-Length") && this->requestHeaders.count("Transfer-Encoding"))
    {
        this->setCodeError(400);
        return false;
    }

    if (this->requestHeaders.count("Content-Length"))
    {
        this->bodyFlag = true;
        ss << this->requestHeaders["Content-Length"];
        ss >> this->bodySize;
        if (this->bodySize == 0)
        {
            this->setCodeError(204);
            return false;
        }
        if (!this->openFile())
            return false;
    }
    if (this->requestHeaders.count("Transfer-Encoding"))
    {
        if (this->requestHeaders["Transfer-Encoding"].find_first_of("chunked") != std::string::npos)
            this->chunkedFlag = true;
        this->bodyFlag = true;
        if (!this->openFile())
            return false;
    }

    if (!this->requestHeaders.count("Host"))
    {
        this->setCodeError(400);
        return false;
    }

    if (this->requestHeaders.count("Host"))
    {
        std::string _host = this->requestHeaders["Host"];
        
        try
        {
            size_t position = _host.find_first_of(':');
            this->Host = skipWhitespaceBeginAnd(_host.substr(0, position));
            std::string host = _host.substr(position + 1, _host.length());
            for (size_t i = 0; i < host.size(); i++)
            {
                if (!std::isdigit(host[i]))
                    this->setCodeError(400);
            }
            this->Port = std::stoul(host);
        }
        catch(const std::exception)
        {
            this->setCodeError(400);
            return false;
        }

    }
    return true;
}

bool                                   Request::keepAlive()
{
    if (this->requestHeaders.count("Connection"))
    {
        if (requestHeaders["Connection"].find("close", 0) != std::string::npos)
            return false;
    }
    return true;
}

void                                   Request::readBufferFromReq(char * buffer, std::size_t readBytes)
{
    u_int8_t                        c;
    static std::stringstream        str;

    for (size_t i = 0; i < readBytes; i++)
    {
        c = buffer[i];
        if (this->getCodeError())
            return;
        switch (this->State)
        {
            case Request_Line:
            {
                if (c == 'G')
                    this->Method = GET;
                else if (c == 'P')
                    this->Method = POST;
                else if (c == 'D')
                    this->Method = DELETE;
                else 
                {
                    this->errorCode = 501;
                    return;
                }
                this->State = Request_Line_Method;
                break;
            }
            case Request_Line_Method: 
            {
                if (c == this->methodsString[this->Method][this->methodIndex])
                    this->methodIndex++;
                else
                {
                    this->errorCode = 501;
                    return ;
                }
                if ((size_t) this->methodIndex == this->methodsString[this->Method].length())
                    this->State = Request_Line_First_Space;
                break ;
            }
            case Request_Line_First_Space:
            {
                if (c != ' ')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_URI_Path_Slash;
                continue;
            }
            case Request_Line_URI_Path_Slash:
            {
                if (c == '/')
                {
                    this->State = Request_Line_URI_Path;
                    this->Storage.clear();
                }
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                break;
            }
            case Request_Line_URI_Path:
            {
                if (c == ' ')
                {
                    this->State = Request_Line_Ver;
                    this->Path.append(this->Storage);
                    this->Storage.clear();
                    continue ;
                }
                else if (c == '?')
                {
                    this->State = Request_Line_URI_Query;
                    this->Path.append(this->Storage);
                    this->Storage.clear();
                    continue ;
                }
                else if (checkUriCharacters(c))
                {
                    this->errorCode = 400;
                    return ;
                }
                else if ( i > MAX_URI_LENGTH)
                {
                    this->errorCode = 414;
                    return ;
                }
                break ;
            }
            case Request_Line_URI_Query:
            {
                if (c == ' ')
                {
                    this->State = Request_Line_Ver;
                    this->Query.append(this->Storage);
                    this->Storage.clear();
                    continue ;
                }
                else if (errorCharQuery(c))
                {
                    this->errorCode = 400;
                    return ;
                }
                else if ( i > MAX_URI_LENGTH)
                {
                    this->errorCode = 414;
                    return ;
                }
                break ;
            }
            case Request_Line_Ver:
            {
                if (errorPath(this->Path))
                {
                    this->errorCode = 400;
                    return ;
                }
                if (c != 'H')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_HT;
                break ;
            }
            case Request_Line_HT:
            {
                if (c != 'T')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_HTT;
                break ;
            }
            case Request_Line_HTT:
            {
                if (c != 'T')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_HTTP;
                break ;

            }
            case Request_Line_HTTP:
            {
                if (c != 'P')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_HTTP_Slash;
                break ;
            }
            case Request_Line_HTTP_Slash:
            {
                if (c != '/')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_Major;
                break ;
            }
            case Request_Line_Major:
            {
                if (!isdigit(c))
                {
                    this->errorCode = 400;
                    return ;
                }
                this->verMajor = c;
                if (this->verMajor > '1')
                    this->errorCode = 505;
                else if (this->verMajor < '1')
                    this->errorCode = 101;
                this->State = Request_Line_Dot;
                break ;
            }
            case Request_Line_Dot:
            {
                if (c != '.')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_Minor;
                break ;
            }
            case Request_Line_Minor:
            {
                if (!isdigit(c))
                {
                    this->errorCode = 400;
                    return ;
                }
                this->verMinor = c;

                if (this->verMinor > '1')
                    this->errorCode = 505;
                else if (this->verMinor < '1')
                    this->errorCode = 101;
                
                this->State = Request_Line_CR;
                break ;
            }
            case Request_Line_CR:
            {
                if (c != '\r')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Request_Line_LF;
                break;
            }
            case Request_Line_LF:
            {
                if (c != '\n')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Field_Name_Start;
                this->Storage.clear();
                continue;
            }
            case Field_Name_Start:
            {
                if (c == '\r')
                    this->State = Field_End;
                else if (checkIsToken(c))
                    this->State = Field_Name;
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                break ;
            }
            case Field_End:
            {
                if (c == '\n')
                {
                    this->Storage.clear();
                    if (!this->handleHeaders())
                        this->State = Parsing_Done;
                    
                    if (this->bodyFlag)
                    {
                        if (this->chunkedFlag == true)
                            this->State = Chunked_Length_Begin;
                        else
                            this->State = Message_Body;
                    }
                    else
                        this->State = Parsing_Done;
                    continue ;
                }
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                break ;
            }
            case Field_Name:
            {
                if (c == ':')
                {
                    this->keyStorage = this->Storage;
                    this->Storage.clear();
                    this->State = Field_Value;
                    continue ;
                }
                else if (!checkIsToken(c))
                {
                    this->errorCode = 400;
                    return ;
                }
                break ;
            }
            case Field_Value:
            {
                if ( c == '\r' )
                {
                    if (checkStringIsEmpty(this->keyStorage) || checkStringIsEmpty(this->Storage))
                        this->errorCode = 400;
                    this->setHeader(this->keyStorage, this->Storage);
                    this->keyStorage.clear();
                    this->Storage.clear();
                    this->State = Field_Value_End;
                    continue ;
                }
                break ;
            }
            case Field_Value_End:
            {
                if ( c == '\n' )
                {
                    this->State = Field_Name_Start;
                    continue ;
                }
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                break ;
            }
            case Chunked_Length_Begin:
            {
                if (isxdigit(c) == 0)
                {
                    this->errorCode = 400;
                    return ;
                }
                str.str("");
                str.clear();
                str << c;
                str >> std::hex >> this->chunkedLength;
                if (this->chunkedLength == 0)
                    this->State = Chunked_Length_CR;
                else
                    this->State = Chunked_Length;
                continue ;
            }
            case Chunked_Length:
            {
                if (isxdigit(c) != 0)
                {
                    int len = 0;
                    str.str("");
                    str.clear();
                    str << c;
                    str >> std::hex >> len;
                    this->chunkedLength *= 16;
                    this->chunkedLength += len;
                }
                else if (c == '\r')
                    this->State = Chunked_Length_LF;
                else
                    this->State = Chunked_Ignore;
                continue ;
            }
            case Chunked_Length_CR:
            {
                if ( c == '\r')
                    this->State = Chunked_Length_LF;
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                continue ;
            }
            case Chunked_Length_LF:
            {
                if ( c == '\n')
                {
                    if (this->chunkedLength == 0)
                        this->State = Chunked_End_CR;
                    else
                        this->State = Chunked_Data;
                }
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                continue ;
            }
            case Chunked_Ignore:
            {
                if (c == '\r')
                    this->State = Chunked_Length_LF;
                continue ;
            }
            case Chunked_Data:
            {
				this->Body.push_back(c);
				--this->chunkedLength;
                if (this->chunkedLength == 0)
                    this->State = Chunked_Data_CR;
				continue ;
            }
            case Chunked_Data_CR:
            {
                if ( c == '\r')
                    this->State = Chunked_Data_LF;
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                continue ;
            }
            case Chunked_Data_LF:
            {
                if ( c == '\n')
                    this->State = Chunked_Length_Begin;
                else
                {
                    this->errorCode = 400;
                    return ;
                }
                continue ;
            }
            case Chunked_End_CR:
            {
                if (c != '\r')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Chunked_End_LF;
                continue ;
            }
            case Chunked_End_LF:
            {
                if (c != '\n')
                {
                    this->errorCode = 400;
                    return ;
                }
                this->State = Parsing_Done;
                continue ;
            }
            case Message_Body:
            {
                if (this->Body.size() < this->bodySize )
                    this->Body.push_back(c);
                if (this->Body.size() == this->bodySize )
                    this->State = Parsing_Done;
                break ;
            }
            case Parsing_Done:
            {
                return ;
            }
        }
        this->Storage += c;
    }
    
    if (this->State == Parsing_Done)
    {
        if (this->getMethodsString() == "POST" && this->fdFileBody > 0)
            write(this->fdFileBody, (char*)this->Body.data(), this->Body.size());
        this->Body.clear();
    }
}

void                                   Request::printRequest()
{
    std::cout << "::::::::::::::::::::::::::::::::::::::::::::::  START PRINTING THE REQUEST  :::::::::::::" << "\n\n";
    std::cout << "METHOD    [" << this->methodsString[this->getMethod()] << "]" << std::endl;
    std::cout << "PATH      [" << this->getPath() << "]" << std::endl;
    std::cout << "VERSION   [" << "HTTP/" << this->verMajor << "." << this->verMinor << "]" << "\n";
    std::cout << "QUERY     [" << this->getQuery() << "]" << std::endl;
    std::cout << "------------------------------ HEADERS ---------------------------------" << "\n";
    for (std::map<std::string, std::string>::iterator it = this->requestHeaders.begin(); it != this->requestHeaders.end(); ++it)
        std::cout << "[" << it->first << "]:[" << it->second << "]" << std::endl;
    std::cout << "------------------------------------------------------------------------" << "\n\n";
    // if (!this->Body.empty() && this->methodsString[this->getMethod()] == "POST")
    // {
    //     std::cout << "------------------------------ BODY ------------------------------------" << "\n";
    //     for (std::vector<u_int8_t>::iterator it = this->Body.begin(); it != this->Body.end(); ++it)
    //         std::cout << *it;
    //     std::cout << "\n------------------------------------------------------------------------" << "\n\n";
    // }
    std::cout << "::::::::::::::::::::::::::::::::::::::::::::::  DONE PRINTING REQUEST  ::::::::::::::::::" << "\n\n";
}

std::string Request::getNewFileName(std::string path)
{
    std::string file_name_never_exist = path;
    std::string exe_flie = this->mime.getExeFile(skipWhitespaceBeginAnd(this->getHeader("Content-Type")));
    if (exe_flie.empty())
    {
        this->setCodeError(415);
        return "";
    }
    
    file_name_never_exist.append(generateRandomFileName());
    file_name_never_exist.append(".");
    file_name_never_exist.append(exe_flie);
    
    if (checkFileExists(file_name_never_exist))
        getNewFileName(path);

    return file_name_never_exist;
}

short                            Request::uploadFile(std::string path_to_upload_file, const ConfigServer & server)
{
    size_t          pos;
    std::string     tmp;

    struct stat st;
	const char *filename = this->nameFileBody.c_str();
	stat(filename, &st);
	off_t size = st.st_size;

    // Request Entity Too Large
    if ( (unsigned long) size > server.getClientMaxBodySize())
        return 413;


    // Check if the folder is exist
    DIR *dir = opendir(path_to_upload_file.c_str());
    if (!dir)
        return 404;
    closedir(dir);
    
    // Check if the file extension is supported
    pos = this->nameFileBody.rfind('.');

    if (pos != std::string::npos)
    {
        try
        {
            tmp = this->nameFileBody.substr(pos + 1, this->nameFileBody.length());

            if (mime.getMimeType(tmp).empty())
                return 415;
        }
        catch(const std::exception& e)
        {
            return 415;
        }   
    }
    else
        return 415;

    pos = this->nameFileBody.rfind('/');

    if (pos == std::string::npos)
        return 500;

    tmp = this->nameFileBody.substr(pos, this->nameFileBody.length());

    if (tmp.empty())
        return 500;
    
    path_to_upload_file += tmp;

    std::rename(this->nameFileBody.c_str(), path_to_upload_file.c_str());

    close(this->fdFileBody);

    return 201;    
}