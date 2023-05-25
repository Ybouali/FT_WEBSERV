# pragma once

# include "../webserv.hpp"

class MimeTypes {

    public :
        MimeTypes();
        ~MimeTypes();
        const std::string getMimeType(const std::string &  );

    private:
        std::map<std::string, std::string>  mime_types;
};