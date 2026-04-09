#include "Network.hpp"

Network::Network():
epoll_fd_(-1), server_vec_(0), client_vec_(0)
{}

Network::Network(std::vector<Server> servers, std::vector<Client> clients):
server_vec_(servers), client_vec_(clients)
{}

Network::Network(Network const& to_copy):
 epoll_fd_(to_copy.epoll_fd_), server_vec_(to_copy.server_vec_), client_vec_(to_copy.client_vec_)
{}

Network const& Network::operator=(Network const& to_copy)
{
    if (this == &to_copy)
        return *this;
    epoll_fd_ = to_copy.epoll_fd_;
    server_vec_ = to_copy.server_vec_;
    client_vec_ = to_copy.client_vec_;
    return *this;
}

void Network::instance_epoll()
{
    
}

void Network::acceptNewClient()
{

}