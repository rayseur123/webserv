#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Server.hpp"
#include <stdint.h>

class Connection
{
    private:
        Server const*   server_;
        int             fd_;
    
    public:

        int             getFd() const;
        Server const&   getServer() const;

        Connection();
        Connection(int connection, Server const& server);
        Connection(Connection const& to_copy);
        Connection const&   operator=(Connection const& to_copy);
        bool            operator==(Connection const& to_comp);
        ~Connection();
    };
    
    std::ostream& operator<<(std::ostream& os, Connection const& connection);

#endif