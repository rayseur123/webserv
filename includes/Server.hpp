#include <string>
#include <vector>
#include "Location.hpp"

class Server
{
    private:
        int fd_;
        int max_client_request_body_;
        std::string address_;
        std::vector<Location>   locations_vec_;
    public:
        int createSocket();

        Server const&   opertor=(Server const& to_copy);

        Server();
        Server(Server const& to_copy);
        ~Server();
}