#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <map>

# include "../response/Response.hpp"
# include "Location.hpp"
#include "../request/Request.hpp"

class Cgi{
    private:
        std::map<std::string, std::string> env_var;
        std::string executable_string;
    public:
        Cgi();
        ~Cgi();
        void put_type_path(Request& r);
        void print_cgi();
        void set_Env(std::string& Method, ServerParser& s, Location& l, Request &r);

};