# include "UploadMultipleFile.hpp"

UploadMultipleFile::UploadMultipleFile()
    : contentDisposition(), contentType(), pathWithFilename(), bodyFile(), vecFiles()
{}

UploadMultipleFile::UploadMultipleFile(const UploadMultipleFile &other)
{
    *this = other;
}

UploadMultipleFile & UploadMultipleFile::operator=(const UploadMultipleFile &other)
{
    if (this != &other)
    {
        this->contentDisposition = other.contentDisposition;
        this->contentType = other.contentType;
        this->pathWithFilename = other.pathWithFilename;
        this->bodyFile = other.bodyFile;
        this->vecFiles = other.vecFiles;
    }
    return *this;
}

UploadMultipleFile::~UploadMultipleFile()
{
    this->clear();
}

void    UploadMultipleFile::clear()
{
    for (size_t i = 0; i < vecFiles.size(); i++)
    {
        this->vecFiles[i].contentDisposition.clear();
        this->vecFiles[i].contentType.clear();   
        this->vecFiles[i].bodyFile.clear();
    }    
    this->vecFiles.clear();
}

const std::string & UploadMultipleFile::getPathWithfilename() const { return this->pathWithFilename; }

const std::vector<u_int8_t> & UploadMultipleFile::getBodyFile() const { return this->bodyFile; }

const std::vector<UploadMultipleFile> & UploadMultipleFile::getVecFiles() const { return this->vecFiles; }

int    UploadMultipleFile::parse_body(const std::string & body, const std::string & boundary)
{
    std::string::size_type  pos = 0;
    UploadMultipleFile      dataFile;
    std::string             fileBody;
    std::string             contentDisposition;
    std::string             contentType;

    while ((pos = body.find(boundary, pos)) != std::string::npos) {
        // Find Content-Disposition
        std::string dispositionStart = "Content-Disposition: ";
        std::string dispositionEnd = "\r\n";
        std::string::size_type dispositionPos = body.find(dispositionStart, pos);
        if (dispositionPos != std::string::npos) {
            dispositionPos += dispositionStart.length();
            std::string::size_type dispositionEndPos = body.find(dispositionEnd, dispositionPos);
            contentDisposition = body.substr(dispositionPos, dispositionEndPos - dispositionPos);
            
        }

        // Find Content-Type
        std::string contentTypeStart = "Content-Type: ";
        std::string contentTypeEnd = "\r\n";
        std::string::size_type contentTypePos = body.find(contentTypeStart, pos);
        if (contentTypePos != std::string::npos) {
            contentTypePos += contentTypeStart.length();
            std::string::size_type contentTypeEndPos = body.find(contentTypeEnd, contentTypePos);
            contentType = body.substr(contentTypePos, contentTypeEndPos - contentTypePos);
        }

        // Find Body
        std::string bodyStart = "\r\n\r\n";
        std::string bodyEnd = "\r\n";
        std::string::size_type bodyPos = body.find(bodyStart, pos);
        if (bodyPos != std::string::npos) {
            bodyPos += bodyStart.length();
            std::string::size_type bodyEndPos = body.find(bodyEnd, bodyPos);
            fileBody = body.substr(bodyPos, bodyEndPos - bodyPos);
        }
        if (!contentDisposition.empty() && !contentType.empty() && !fileBody.empty())
        {
            // dataFile.
            std::cout << "{" << contentDisposition << "}" << std::endl << std::endl;
            std::cout << "{" << contentType << "}" << std::endl << std::endl;
            std::cout << "{" << fileBody << "}" << std::endl << std::endl;
        }
        contentDisposition.clear();
        contentType.clear();
        fileBody.clear();
        
        pos = pos + boundary.length();
    }
    return 0;
}