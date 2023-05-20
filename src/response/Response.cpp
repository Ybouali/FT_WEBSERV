#include "Response.hpp"

Response::Response() :
	server(),
	request(),
	responseContent(),
	protocol("HTTP/1.1"),
	statusCode(),
	statusMessage(),
	contentType(),
	contentLength(),
	serverName("nginx/13.37"),
	Date()
{}

Response::~Response()
{
    this->clear();
}

void	Response::clear()
{
    this->server.clear();
    this->request.clear();
	this->responseContent.clear();
	this->statusCode.clear();
	this->statusMessage.clear();
	this->contentType.clear();
	this->contentLength.clear();
	this->Date.clear();
}

// ----------------------------- Getters -----------------------------------

const ConfigServer	Response::getConfigServer() const { return this->server; }

const Request		Response::getRequest() const { return this->request; }

const std::string&	Response::getResponseContent() const { return this->responseContent; }

const std::string&	Response::getProtocol() const { return this->protocol; }

const std::string&	Response::getStatusCode() const { return this->statusCode; }

const std::string&	Response::getStatusMessage() const { return this->statusMessage; }

const std::string&	Response::getContentType() const { return this->contentType; }

const std::string&	Response::getContentLength() const { return this->contentLength; }

const std::string&	Response::getServerName() const { return this->serverName; }

const std::string&	Response::getDate() const { return this->Date; }

// ----------------------------- Setters -----------------------------------

void	Response::setConfigServer(const ConfigServer& server) { this->server = server; }

void	Response::setRequest(const Request& request) { this->request = request; }

void	Response::setResponseContent(const std::string& responseContent) { this->responseContent = responseContent; }

void	Response::setStatusCode(const std::string& statusCode) { this->statusCode = statusCode; }

void	Response::setStatusMessage(const std::string& statusMessage) { this->statusMessage = statusMessage; }

void	Response::setContentType(const std::string& contentType) { this->contentType = contentType; }

void	Response::setContentLength(const std::string& contentLength) { this->contentLength = contentLength; }

void	Response::setDate(const std::string& Date) { this->Date = Date; }

// ----------------------------- Methodes -----------------------------------

// std::string	getDayName(int dayIndex)
// {
//     const std::string dayNames[] = {
//         "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
//     };
//     return dayNames[dayIndex];
// }

// void	Response::getdatetime()
// {
//     Response r;

//     time_t currentTime = time(NULL);
//     tm* utcTime = gmtime(&currentTime);

//     // Convert the month index to the corresponding month name
//     const std::string monthNames[] = {
//         "Jan", "Feb", "Mar", "Apr", "May", "Jun",
//         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
//     };
//     const std::string& monthName = monthNames[utcTime->tm_mon];

//     // Create the formatted date string
//     std::string dateString = "Date: " +
//         getDayName(utcTime->tm_wday) + ", " +
//         std::to_string(utcTime->tm_mday) + " " +
//         monthName + " " +
//         std::to_string(utcTime->tm_year + 1900) + " " +
//         std::to_string(utcTime->tm_hour) + ":" +
//         std::to_string(utcTime->tm_min) + ":" +
//         std::to_string(utcTime->tm_sec) + " UTC\r\n\r\n";

//     // Print the formatted date string
//     std::cout << dateString << std::endl;

//     r.setDate(dateString);
// }

void	Response::createResponse()
{
}
