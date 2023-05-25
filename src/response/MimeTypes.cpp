#include "MimeTypes.hpp"

MimeTypes::MimeTypes()
    : mime_types()
{
    this->mime_types[".aac"] = "audio/aac";
    this->mime_types[".abw"] = "application/x-abiword";
    this->mime_types[".arc"] = "application/x-freearc";
    this->mime_types[".avi"] = "video/x-msvideo";
    this->mime_types[".azw"] = "application/vnd.amazon.ebook";
    this->mime_types[".bin"] = "application/octet-stream";
    this->mime_types[".bmp"] = "image/bmp";
    this->mime_types[".bz"] = "application/x-bzip";
    this->mime_types[".bz2"] = "application/x-bzip2";
    this->mime_types[".csh"] = "application/x-csh";
    this->mime_types[".css"] = "text/css";
    this->mime_types[".csv"] = "text/csv";
    this->mime_types[".doc"] = "application/msword";
    this->mime_types[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    this->mime_types[".eot"] = "application/vnd.ms-fontobject";
    this->mime_types[".epub"] = "application/epub+zip";
    this->mime_types[".gz"] = "application/gzip";
    this->mime_types[".gif"] = "image/gif";
    this->mime_types[".htm"] = "text/html";
    this->mime_types[".html"] = "text/html";
    this->mime_types[".ico"] = "image/vnd.microsoft.icon";
    this->mime_types[".ics"] = "text/calendar";
    this->mime_types[".jar"] = "application/java-archive";
    this->mime_types[".jpeg"] = "image/jpeg";
    this->mime_types[".jpg"] = "image/jpeg";
    this->mime_types[".js"] = "text/javascript";
    this->mime_types[".json"] = "application/json";
    this->mime_types[".jsonld"] = "application/ld+json";
    this->mime_types[".mid"] = "audio/midi";
    this->mime_types[".midi"] = "audio/midi";
    this->mime_types[".mjs"] = "text/javascript";
    this->mime_types[".mp3"] = "audio/mpeg";
    this->mime_types[".mpeg"] = "video/mpeg";
    this->mime_types[".mpkg"] = "application/vnd.apple.installer+xml";
    this->mime_types[".odp"] = "application/vnd.oasis.opendocument.presentation";
    this->mime_types[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    this->mime_types[".odt"] = "application/vnd.oasis.opendocument.text";
    this->mime_types[".oga"] = "audio/ogg";
    this->mime_types[".ogv"] = "video/ogg";
    this->mime_types[".ogx"] = "application/ogg";
    this->mime_types[".opus"] = "audio/opus";
    this->mime_types[".otf"] = "font/otf";
    this->mime_types[".png"] = "image/png";
    this->mime_types[".pdf"] = "application/pdf";
    this->mime_types[".php"] = "application/x-httpd-php";
    this->mime_types[".ppt"] = "application/vnd.ms-powerpoint";
    this->mime_types[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    this->mime_types[".rar"] = "application/vnd.rar";
    this->mime_types[".rtf"] = "application/rtf";
    this->mime_types[".sh"] = "application/x-sh";
    this->mime_types[".svg"] = "image/svg+xml";
    this->mime_types[".swf"] = "application/x-shockwave-flash";
    this->mime_types[".tar"] = "application/x-tar";
    this->mime_types[".tif"] = "image/tiff";
    this->mime_types[".tiff"] = "image/tiff";
    this->mime_types[".ts"] = "video/mp2t";
    this->mime_types[".ttf"] = "font/ttf";
    this->mime_types[".txt"] = "text/plain";
    this->mime_types[".vsd"] = "application/vnd.visio";
    this->mime_types[".wav"] = "audio/wav";
    this->mime_types[".weba"] = "audio/webm";
    this->mime_types[".webm"] = "video/webm";
    this->mime_types[".webp"] = "image/webp";
    this->mime_types[".woff"] = "font/woff";
    this->mime_types[".woff2"] = "font/woff2";
    this->mime_types[".xhtml"] = "application/xhtml+xml";
    this->mime_types[".xls"] = "application/vnd.ms-excel";
    this->mime_types[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    this->mime_types[".xml"] = "application/xml";
    this->mime_types[".xul"] = "application/vnd.mozilla.xul+xml";
    this->mime_types[".zip"] = "application/zip";
    this->mime_types[".3gp"] = "video/3gpp";
    this->mime_types[".3g2"] = "video/3gpp2";
    this->mime_types[".7z"] = "application/x-7z-compressed";
}
MimeTypes::~MimeTypes()
{
    this->mime_types.clear();
}

const std::string MimeTypes::getMimeType(const std::string & exe)
{
    if (this->mime_types.count(exe))
        return this->mime_types[exe];
    return "text/html";
}