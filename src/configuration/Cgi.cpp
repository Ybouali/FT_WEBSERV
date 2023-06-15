//need to check request method , query string ,
// content type and content length.

#include "Cgi.hpp"

std::string getFileExtensionFromPath(const std::string& path)
{
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos && dotPos < path.length() - 1)
    {
        return path.substr(dotPos + 1);
    }
    return "";
}

void Cgi::put_type_path(Request& r){
    std::string type = getFileExtensionFromPath(r.getPath());
    if (type == "php")
        executable_string = "/Users/zezzine/Desktop/FT_WEBSERV/cgi/php-cgi";
    else if (type == "py")
        executable_string = "/Users/zezzine/Desktop/FT_WEBSERV/cgi/python";
    if (r.getQuery() != ""){
        if (r.getQuery().at(1) != '&'){
            std::string tmp = r.getQuery().substr(1);
            r.getQuery() = "?&" + tmp;
        }
    }
}

void Cgi::set_Env(std::string& Method, ServerParser& s, Location& l, Request& r){
    env_var["GATEWAY_INTERFACE"] = "CGI/1.1";
    env_var["PATH_INFO"] = r.getPath();
    // env_var["PATH_TRANSLATED"] = ;
    env_var["REDIRECT_STATUS"] = "200";
    env_var["REQUEST_METHOD"] = Method;
    env_var["SERVER_NAME"] = s.getServerName();
    env_var["SERVER_PORT"] = s.getPort();
    env_var["SERVER_PROTOCOL"] = "HTTP/1.1";
    if (Method == "GET")
        env_var["QUERY_STRING"] = r.getQuery();
}