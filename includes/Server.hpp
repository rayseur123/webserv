#ifndef SERVER_CPP
#define SERVER_CPP

#include <string>
#include <vector>
#include <utility>
#include "Location.hpp"

#define LISTEN_QUEUE 10

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
        
        void                            createSocket();
        void                            setNoBlockingFd();
        
        int const&                      getFd() const;
        int const&                      getMaxClientRequestBody() const; 
        std::string const&              getPort() const;
        std::vector<Location> const&    getLocation() const;
        
        Server const&   operator=(Server const& to_copy);

        void            setLocations(std::vector<Location> const& location_vec);
        void            setMaxClientRequestBody(std::string const& max_client_request_body);
        void            setAddrAndPort(std::string const& addr_and_port);
        void            setErrorPage(std::vector<std::string> const& error_page);

        int                                 getMaxClientRequestBody() const;
        std::string const&                  getAddress() const;
        std::string const&                  getPort() const;
        std::vector<Location> const&        getLocations() const;
        std::pair<int, std::string> const&  getErrorPage() const;

        Server();
        Server(Server const& to_copy);
        Server(int fd, int max_client_request_body, std::string const& address,
            std::string const& port, std::vector<Location> const& locations_vec);
        ~Server();
};

std::ostream&   operator<<(std::ostream& os, Server const& to_print);

#endif