#ifndef SERVER_CPP
#define SERVER_CPP

#include <string>
#include <vector>
#include "Location.hpp"

class Server
{
    private:
        int                     fd_;
        int                     max_client_request_body_;
        std::string             address_;
        int                     port_;
        std::vector<Location>   locations_vec_;
    public:
        int createSocket();

        Server const&   operator=(Server const& to_copy);

        Server();
        Server(Server const& to_copy);
        Server(int fd, int max_client_request_body, std::string address,
            int port, std::vector<Location> locations_vec);
        ~Server();
};

#endif