#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include "Server.hpp"

class Client
{
    private:
        
        Server  server_;
        int     client_fd_;
    
    public:

        int             getClient() const;
        Server const&   getServer() const;

        Client();
        Client(int client, Server const& server);
        Client(Client const& to_copy);
        Client const& operator=(Client const& to_copy);
        ~Client();
    };
    
    std::ostream& operator<<(std::ostream& os, Client const& client);

#endif