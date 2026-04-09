#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <Client.hpp>
#include <Server.hpp>

class Network
{
    private:
        int                 epoll_fd_;    
        std::vector<Server> server_vec_;
        std::vector<Client> client_vec_;

    public:

        void instance_epoll();
        void acceptNewClient();

        Network();
        Network(std::vector<Server> servers, std::vector<Client> clients);
        Network(Network const& to_copy);
        Network const& operator=(Network const& to_copy);
        ~Network();
};

#endif