# include "UploadMultipleFile.hpp"

MimeTypes UploadMultipleFile::mime;

UploadMultipleFile::UploadMultipleFile()
    : codeStatus(0), contentDisposition(), contentType(), curr_fileName(), fileName(), pathWithFilename(), bodyFile(), vecFiles()
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
        this->curr_fileName = other.curr_fileName;
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
        this->vecFiles[i].curr_fileName.clear();
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

void                                        UploadMultipleFile::setPathWithfilename(const std::string & path)
{
    if (!this->contentDisposition.empty())
    {
        // Find the key file name 
        // Example : Content-Disposition: form-data; name="file1"; filename="file1.txt"
        size_t startPosName = this->contentDisposition.find_first_of('=') + 2;
        size_t len = this->contentDisposition.find_first_of('"', startPosName) - startPosName;

        this->fileName = this->contentDisposition.substr(startPosName, len);

        if (this->fileName.empty())
        {
            try
            {
                std::cout << this->contentDisposition << std::endl;
                startPosName = this->contentDisposition.find("filename=\"") + 10;
                len = this->contentDisposition.find_first_of('"', startPosName) - startPosName;
                this->curr_fileName = this->contentDisposition.substr(startPosName, len);
            }
            catch(const std::exception& e)
            {
                this->fileName = generateRandomFileName();
            }
        }
    }

    // if there is no file name so the new file will be named like the uploaded file name
    if (this->fileName.empty())
        this->fileName = this->curr_fileName;
    else
    {
        // if the new file name exists so we will need to add the extension
        this->fileName += mime.getMimeType(this->contentType);
    }
    
    // HERE we need to join the path with the file name
    this->pathWithFilename = path + this->fileName;
}

const std::vector<UploadMultipleFile> &    UploadMultipleFile::parse_body(const std::string & body, const std::string & boundary, const std::string & path)
{
    std::string::size_type  pos = 0;
    std::string             fileBody;
    std::string             contentDisposition;
    std::string             contentType;
    short                   codeError = 0;

    while ((pos = body.find(boundary, pos)) != std::string::npos) {
        // Find Content-Disposition
        std::string dispositionStart = "Content-Disposition: ";
        std::string dispositionEnd = "\r\n";
        std::string::size_type dispositionPos = body.find(dispositionStart, pos);
        if (dispositionPos != std::string::npos) {
            dispositionPos += dispositionStart.length();
            std::string::size_type dispositionEndPos = body.find(dispositionEnd, dispositionPos);
            try
            {
                contentDisposition = body.substr(dispositionPos, dispositionEndPos - dispositionPos);
            }
            catch(const std::exception)
            {
                contentDisposition.clear();
            }
            if (contentDisposition.empty())
                codeError = 409;
            
        }

        // Find Content-Type
        std::string contentTypeStart = "Content-Type: ";
        std::string contentTypeEnd = "\r\n";
        std::string::size_type contentTypePos = body.find(contentTypeStart, pos);
        if (contentTypePos != std::string::npos) {
            contentTypePos += contentTypeStart.length();
            std::string::size_type contentTypeEndPos = body.find(contentTypeEnd, contentTypePos);
            
            try
            {
                contentType = body.substr(contentTypePos, contentTypeEndPos - contentTypePos);
            }
            catch(const std::exception)
            {
                contentType.clear();
            }
        }

        // Find Body
        std::string bodyStart = "\r\n\r\n";
        std::string bodyEnd = boundary;
        std::string::size_type bodyPos = body.find(bodyStart, pos);
        if (bodyPos != std::string::npos) {
            bodyPos += bodyStart.length();
            std::string::size_type bodyEndPos = body.find(bodyEnd, bodyPos);
            try
            {
                fileBody = body.substr(bodyPos, (bodyEndPos - bodyPos - 5));
            }
            catch(const std::exception)
            {
                fileBody.clear();
            }
            if (fileBody.empty())
                codeError = 204;
            
        }

        if (!contentDisposition.empty() && !fileBody.empty())
        {
            UploadMultipleFile      dataFile;
            
            dataFile.setContentDisposition(contentDisposition);
            dataFile.setContentType(contentType);
            dataFile.setBodyFile(fileBody);
            dataFile.codeStatus = codeError;
            if (!dataFile.codeStatus)
                dataFile.setPathWithfilename(path);
            this->vecFiles.push_back(dataFile);
            if (dataFile.codeStatus)
                return this->vecFiles;
        }

        contentDisposition.clear();
        contentType.clear();
        fileBody.clear();
        
        pos = pos + boundary.length();
    }
    return this->vecFiles;
}
