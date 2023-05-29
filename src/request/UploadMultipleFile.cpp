# include "UploadMultipleFile.hpp"

MimeTypes UploadMultipleFile::mime;

UploadMultipleFile::UploadMultipleFile()
    : codeStatus(0), contentDisposition(), contentType(), pathWithFilename(), bodyFile(), vecFiles()
{}

UploadMultipleFile::UploadMultipleFile(const UploadMultipleFile &other)
{
    *this = other;
}

UploadMultipleFile & UploadMultipleFile::operator=(const UploadMultipleFile &other)
{
    if (this != &other)
    {
        this->codeStatus = other.codeStatus;
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

const std::string &                         UploadMultipleFile::getPathWithfilename() const { return this->pathWithFilename; }

const std::string &                         UploadMultipleFile::getBodyFile() const { return this->bodyFile; }

const std::vector<UploadMultipleFile> &     UploadMultipleFile::getVecFiles() const { return this->vecFiles; }

void                                        UploadMultipleFile::setBodyFile(const std::string & _bodyFile) { this->bodyFile = _bodyFile; }

void                                        UploadMultipleFile::setContentDisposition(const std::string & _contentDisposition) { this->contentDisposition = _contentDisposition; }

void                                        UploadMultipleFile::setContentType(const std::string & _contentType) { this->contentType = _contentType; };

void                                        UploadMultipleFile::setPathWithfilename(const std::string & path) 
{
    // this->codeStatus = 0;
    std::string filename;

    size_t startPosName = this->contentDisposition.find("name") + 6;

    size_t i = startPosName;
    while (this->contentDisposition[i] != '"')
        filename += this->contentDisposition[i++];
    
    if (filename.empty())
        filename = generateRandomFileName();
    if (mime.getMimeType(this->contentType).empty())
        this->codeStatus = 415;
    else
        filename += mime.getMimeType(this->contentType);
    this->pathWithFilename = path;
    this->pathWithFilename += filename;
}

int    UploadMultipleFile::parse_body(const std::string & body, const std::string & boundary, const std::string & path)
{
    std::string::size_type  pos = 0;
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
            UploadMultipleFile      dataFile;
            
            dataFile.setContentDisposition(contentDisposition);
            dataFile.setContentType(contentType);
            dataFile.setBodyFile(fileBody);
            dataFile.setPathWithfilename(path);
            this->vecFiles.push_back(dataFile);
        }
        contentDisposition.clear();
        contentType.clear();
        fileBody.clear();
        
        pos = pos + boundary.length();
    }
    return 0;
}
