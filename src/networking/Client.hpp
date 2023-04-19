# pragma once

# include "../webserv.hpp"
# include "../request/Request.hpp"
# include "../configuration/ConfigServer.hpp"

class Client
{
    private:
        int                         clientSocket;
        time_t                      lastMsgTime;
        struct sockaddr_in          clientAddress;
    public:
        Client();

        Client(const ConfigServer &);
        
        ~Client();
        
        Request         request;
        ConfigServer    server;

        // ? ----------------------------- getters -----------------------------------

        int                     getClientSocket() const;

        time_t                  getLastMsgTime() const;

        struct sockaddr_in      getClientAddress() const;

        // ? ----------------------------- setters -----------------------------------

        void                    setClientSocket(int);

        void                    setLastMsgTime(time_t);

        void                    setClientAddress(struct sockaddr_in);

        // ? Methodes ----------------------------------------------------------------

        void                    updateTime();
};