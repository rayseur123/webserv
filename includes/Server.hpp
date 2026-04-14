#ifndef SERVER_CPP
#define SERVER_CPP

#include <string>
#include <vector>
#include "Location.hpp"

#define LISTEN_QUEUE 10

class Server
{
    private:
        int fd_;
        int max_client_request_body_;
        std::string port_;
        std::string address_;
        std::vector<Location>   locations_vec_;
    public:
        
        void                            createSocket();
        void                            setNoBlockingFd();
        
        int const&                      getFd() const;
        int const&                      getMaxClientRequestBody() const; 
        std::string const&              getPort() const;
        std::vector<Location> const&    getLocation() const;
        
        Server const&   operator=(Server const& to_copy);

        Server();
        Server(Server const& to_copy);
        Server(int max_client_request_body, std::string port_, std::string address,
            std::vector<Location>& locations_vec);
        ~Server();
};

Server& findServerByFd(std::vector<Server>& server_vec, int&  server_fd);

#endif