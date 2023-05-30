# include "UploadMultipleFile.hpp"

MimeTypes UploadMultipleFile::mime;

UploadMultipleFile::UploadMultipleFile()
    : codeStatus(0), contentDisposition(), contentType(), fileName(), pathWithFilename(), bodyFile(), vecFiles()
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
        this->bodyFile = other.bodyFile;
        this->contentType = other.contentType;
        this->fileName = other.fileName;
        this->pathWithFilename = other.pathWithFilename;
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
        this->vecFiles[i].fileName.clear();
        this->vecFiles[i].bodyFile.clear();
    }    
    this->vecFiles.clear();
}

const std::string &                         UploadMultipleFile::getFileName() const { return this->fileName; }

const std::string &                         UploadMultipleFile::getPathWithfilename() const { return this->pathWithFilename; }

const std::string &                         UploadMultipleFile::getBodyFile() const { return this->bodyFile; }

const std::vector<UploadMultipleFile> &     UploadMultipleFile::getVecFiles() const { return this->vecFiles; }

void                                        UploadMultipleFile::setBodyFile(const std::string & _bodyFile) { this->bodyFile = _bodyFile; }

void                                        UploadMultipleFile::setContentDisposition(const std::string & _contentDisposition) { this->contentDisposition = _contentDisposition; }

void                                        UploadMultipleFile::setContentType(const std::string & _contentType) { this->contentType = _contentType; };

void                                        UploadMultipleFile::setPathWithfilename(const std::string & path, const std::vector<UploadMultipleFile> & files)
{
    size_t startPosName = this->contentDisposition.find("name") + 6;

    size_t i = startPosName;
    while (this->contentDisposition[i] != '"')
        this->fileName += this->contentDisposition[i++];
    
    if (mime.getMimeType(this->contentType).empty())
        this->codeStatus = 415;
    else
    {
        if (this->fileName.empty())
            this->fileName = generateRandomFileName();
        this->fileName += mime.getMimeType(this->contentType);
        
        for (size_t i = 0; i < files.size(); i++)
        {
            if (this->fileName == files[i].getFileName())
                this->fileName.clear();
        }
        if (this->fileName.empty())
        {
            this->fileName = generateRandomFileName();
            this->fileName += mime.getMimeType(this->contentType);
        }
    }
    
    this->pathWithFilename = path;
    this->pathWithFilename += this->fileName;
}

const std::vector<UploadMultipleFile> &    UploadMultipleFile::parse_body(const std::string & body, const std::string & boundary, const std::string & path)
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
        std::string bodyEnd = boundary;
        std::string::size_type bodyPos = body.find(bodyStart, pos);
        if (bodyPos != std::string::npos) {
            bodyPos += bodyStart.length();
            std::string::size_type bodyEndPos = body.find(bodyEnd, bodyPos);
            fileBody = body.substr(bodyPos, (bodyEndPos - bodyPos - 5));
        }
        if (!contentDisposition.empty() && !contentType.empty() && !fileBody.empty())
        {
            UploadMultipleFile      dataFile;
            
            dataFile.setContentDisposition(contentDisposition);
            dataFile.setContentType(contentType);
            dataFile.setBodyFile(fileBody);
            dataFile.setPathWithfilename(path, this->vecFiles);
            this->vecFiles.push_back(dataFile);
        }
        contentDisposition.clear();
        contentType.clear();
        fileBody.clear();
        
        pos = pos + boundary.length();
    }
    return this->vecFiles;
}
