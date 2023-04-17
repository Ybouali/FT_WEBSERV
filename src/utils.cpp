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