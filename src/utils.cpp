#include "webserv.hpp"

std::string     skipWhitespaceBeginAnd(std::string line)
{
    line.erase(line.begin(), std::find_if_not(line.begin(), line.end(), isspace));
    line.erase(std::find_if_not(line.rbegin(), line.rend(), isspace).base(), line.end());

    return line;
}
