#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client
{
    private:
        
        Server const*   server_;
        int             fd_;
    
    public:

        int             getFd() const;
        Server const&   getServer() const;

        Client();
        Client(int client, Server const& server);
        Client(Client const& to_copy);
        Client const&   operator=(Client const& to_copy);
        bool            operator==(Client const& to_comp);
        ~Client();
    };
    
    std::ostream& operator<<(std::ostream& os, Client const& client);

#endif