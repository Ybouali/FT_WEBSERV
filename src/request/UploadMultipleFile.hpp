# pragma once

#include "../webserv.hpp"

class UploadMultipleFile {

    public :
        UploadMultipleFile();
        ~UploadMultipleFile();
        UploadMultipleFile(const UploadMultipleFile &other);
        UploadMultipleFile &operator=(const UploadMultipleFile &other);

        void   clear();


        const std::string &                         getPathWithfilename() const;
        const std::vector<u_int8_t> &               getBodyFile() const;
        const std::vector<UploadMultipleFile> &     getVecFiles() const;
        int             parse_body(const std::string & , const std::string & );

    private :
        std::string                         contentDisposition;
        std::string                         contentType;
        std::string                         pathWithFilename;
        std::vector<u_int8_t>               bodyFile;
        std::vector<UploadMultipleFile>     vecFiles;

};