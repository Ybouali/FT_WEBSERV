#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <ctime>
#include <string>

class Response
{
    private:
        /* data */
        std::string Date;
    public:
        Response(/* args */);
        ~Response();

        //getters
        const std::string& getDate() const;

        //setters
        void setDate(const std::string& );

        //functions
        void getdatetime();
};
#endif