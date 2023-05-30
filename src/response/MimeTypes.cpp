#include "MimeTypes.hpp"

MimeTypes::MimeTypes()
    : mime_types()
{
    this->mime_types["text/html"] = ".html";
    this->mime_types["text/css"] = ".css";
    this->mime_types["image/x-icon"] = ".ico";
    this->mime_types["video/x-msvideo"] = ".avi";
    this->mime_types["image/bmp"] = ".bmp";
    this->mime_types["application/msword"] = ".doc";
    this->mime_types["image/gif"] = ".gif";
    this->mime_types["application/x-gzip"] = ".gz";
    this->mime_types["image/x-icon"] = ".ico";
    this->mime_types["image/jpeg"] = ".jpg";
    this->mime_types["image/png"] = ".png";
    this->mime_types["text/plain"] = ".txt";
    this->mime_types["audio/mp3"] = ".mp3";
    this->mime_types["application/pdf"] = ".pdf";
    this->mime_types["video/mp4"] = ".mp4";
}
MimeTypes::~MimeTypes()
{
    this->mime_types.clear();
}

const std::string MimeTypes::getMimeType(const std::string & exe)
{
    if (this->mime_types.count(exe))
        return this->mime_types[exe];
    return "";
}