# pragma once

#include "../webserv.hpp"

class UploadMultipleFile {

    public :
        static MimeTypes    mime;
        short               codeStatus;
        UploadMultipleFile();
        ~UploadMultipleFile();
        UploadMultipleFile(const UploadMultipleFile &other);
        UploadMultipleFile &operator=(const UploadMultipleFile &other);

        void   clear();


        const std::string &                         getPathWithfilename() const;
        const std::string &                         getBodyFile() const;
        const std::vector<UploadMultipleFile> &     getVecFiles() const;
        const std::string &                         getFileName() const;

        void                                        setContentDisposition(const std::string & );
        void                                        setContentType(const std::string & );
        void                                        setPathWithfilename(const std::string & );
        void                                        setBodyFile(const std::string &);

        const std::vector<UploadMultipleFile> &     parse_body(const std::string & , const std::string & , const std::string &);

    private :
        std::string                         contentDisposition;
        std::string                         contentType;
        std::string                         curr_fileName;
        std::string                         fileName;
        std::string                         pathWithFilename;
        std::string                         bodyFile;
        std::vector<UploadMultipleFile>     vecFiles;
};