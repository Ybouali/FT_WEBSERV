#include "Response.hpp"

Response::Response() 
    : response_content(), server(), request()
{ }

Response::~Response()
{
    this->clear();
}

void Response::clear()
{
    this->response_content.clear();
    this->server.clear();
    this->request.clear();
}

void            Response::setRequest(Request req)
{
    this->request = req;
}

Request         Response::getRequest() const { return this->request; }

void            Response::setConfigServer(ConfigServer ser)
{
    this->server = ser;
}
ConfigServer    Response::getConfigServer() const { return this->server; }

// // const std::string& Response::getDate() const{
// //     return Date;
// // }

// // void Response::setDate(const std::string& value){
// //     this->Date = value;
// // }

// std::string getDayName(int dayIndex) {
//     const std::string dayNames[] = {
//         "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
//     };
//     return dayNames[dayIndex];
// }

// void Response::getdatetime(){
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

// Response::Response(){}
// Response::~Response(){}