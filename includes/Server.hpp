#ifndef SERVER_CPP
#define SERVER_CPP

#include <string>
#include <vector>
#include <utility>
#include "Location.hpp"

class Server
{
    private:
        int                         fd_;
        
        int                         max_client_request_body_;
        std::string                 address_;
        std::string                 port_;
        std::vector<Location>       locations_vec_;
        std::pair<int, std::string> error_page_;
    public:
        int createSocket();

        Server const&   operator=(Server const& to_copy);

        void            setLocations(std::vector<Location> const& location_vec);
        void            setMaxClientRequestBody(std::string const& max_client_request_body);
        void            setAddrAndPort(std::string const& addr_and_port);
        void            setErrorPage(std::vector<std::string> const& error_page);

        void print() const;

        Server();
        Server(Server const& to_copy);
        Server(int fd, int max_client_request_body, std::string address,
            std::string port, std::vector<Location> locations_vec);
        ~Server();
};

#endif