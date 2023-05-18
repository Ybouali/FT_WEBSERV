#pragma once

# define CLIENT_MAX_BODY_SIZE 3000000

# define MAX_URI_LENGTH 4096

# define TIMEOUT_CONNECTION 60

# define MSG_BUF 40960

# include <iostream>
# include <fstream>
# include <fcntl.h>
# include <cstring>
# include <string> 
# include <unistd.h>
# include <dirent.h>
# include <sstream>

# include <cstdlib>
# include <fstream>
# include <sstream>
# include <cctype>
# include <ctime>
# include <cstdarg>

/* STL Containers */
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>

/* System */
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>
# include <signal.h>

/* Network */
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <arpa/inet.h>

#include <regex>

# include "configuration/Location.hpp"
# include "configuration/ServerParser.hpp"
# include "configuration/ConfigServer.hpp"
# include "request/Request.hpp"

// ? UTILS FUNCTIONS

std::string     skipWhitespaceBeginAnd(std::string );
bool            errorPath(std::string );
bool            checkUriCharacters(u_int8_t );
bool            checkIsToken(u_int8_t );
std::string     getPageError(short );
std::string     getIndexHomePageSmallNginx();

