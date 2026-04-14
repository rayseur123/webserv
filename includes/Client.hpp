#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include "Server.hpp"

class Client
{
    private:
        
        int client_fd_;
        Server server_;
    
    public:

        int const& getClient() const;
        Server const& getServer() const;

        Client();
        Client(int client, Server const& server);
        Client(Client const& to_copy);
        Client const& operator=(Client const& to_copy);
        ~Client();
    };
    

#endif