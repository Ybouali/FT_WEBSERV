#include "webserv.hpp"

std::string     skipWhitespaceBeginAnd(std::string line)
{
    line.erase(line.begin(), std::find_if_not(line.begin(), line.end(), isspace));
    line.erase(std::find_if_not(line.rbegin(), line.rend(), isspace).base(), line.end());
    return line;
}

bool            errorPath(std::string path)
{
    std::string     t(path);
    char *r = strtok((char*)t.c_str(), "/");
    int p = 0;
    while (r != NULL)
    {
        if (!strcmp(r, ".."))
            p--;
        else
            p++;
        if (p < 0)
            return true;
        r = strtok(NULL, "/");
    }
    return false;
}

bool            checkUriCharacters(u_int8_t c)
{
    if ((c >= '#' && c <= ';') || (c >= '?' && c <= '[') || (c >= 'a' && c <= 'z') || \
       c == '!' || c == '=' || c == ']' || c == '_' || c == '~')
        return (true);
    return (false);
}

bool           checkIsToken(u_int8_t c)
{
    if (c == '!' || (c >= '#' && c <= '\'') || c == '*'|| c == '+' || c == '-'  || c == '.' || \
       (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= '^' && c <= '`') || \
       (c >= 'a' && c <= 'z') || c == '|')
        return (true);
    return (false);
}

std::string statusCodeString(short statusCode)
{
    switch (statusCode)
    {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocol";
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        case 300:
            return "Multiple Choice";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Moved Temporarily";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Requested Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 418:
            return "I'm a teapot";
        case 421:
            return "Misdirected Request";
        case 425:
            return "Too Early";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        case 451:
            return "Unavailable for Legal Reasons";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 506:
            return "Variant Also Negotiates";
        case 507:
            return "Insufficient Storage";
        case 510:
            return "Not Extended";
        case 511:
            return "Network Authentication Required";
        default:
            return "Undefined";
    }
}

std::string getPageError(short statusCode)
{
    std::string page = "";
    std::string s_code = std::to_string(statusCode);
    std::string s_code_string = statusCodeString(statusCode);
    page.append("<!DOCTYPE html>\r\n<html>\r\n<head><title> small nginx </title> \r\n <style>\r\nbody { \r\n width: 35em;\r\nmargin: 0 auto; \r\n color: #272727; background-color: #3366CC; \r\nfont-family: new century, schoolbook;\r\n }\r\n</style></head>\r\n <body>\r\n <center> \r\n <h1> WELCOM TO THE SMALL NGINX </h1> \r\n <h2>");
    page.append(s_code);
    page.append(" status code and the message is [");
    page.append(s_code_string);
    page.append("]</h2> </center>\r\n</body>\r\n </html>");
    return page;
}

std::string	getDateFormat()
{
	time_t	currentTime = time(NULL);
	tm*		utcTime = gmtime(&currentTime);

	std::string monthNames[] = {
	    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	std::string monthName = monthNames[utcTime->tm_mon];

	std::string dayNames[] = {
	    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	std::string dayName = dayNames[utcTime->tm_wday];

	std::string date = "Date: " + dayName + ", " +
	std::to_string(utcTime->tm_mday) + " " + monthName + " " +
	std::to_string(utcTime->tm_year + 1900) + " " +
	std::to_string(utcTime->tm_hour) + ":" +
	std::to_string(utcTime->tm_min) + ":" +
	std::to_string(utcTime->tm_sec) + " UTC";

	return (date);
}
