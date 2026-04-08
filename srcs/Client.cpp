#include "Client.hpp"
#include <iostream>

int findServerByClient(std::vector<Client> client_vec, int client_fd)
{
    for (std::size_t i = 0; i < client_vec.size(); i++)
    {
        if (client_vec[i].getClient() == client_fd)
            return (client_vec[i].getServer());
    }
    return (-1);
}

int const& Client::getClient() const
{
    return (client_fd_);
}

int const& Client::getServer() const
{
    return (server_fd_);
}

Client::Client()
: client_fd_(-1), server_fd_(-1)
{}

Client::Client(int client, int server)
: client_fd_(client), server_fd_(server)
{}

Client::Client(Client const& to_copy)
: client_fd_(to_copy.client_fd_), server_fd_(to_copy.server_fd_)
{}

Client const& Client::operator=(Client const& to_copy)
{
    if (this == &to_copy)
        return (*this);
    client_fd_ = to_copy.client_fd_;
    server_fd_ = to_copy.server_fd_;
    return (*this);
}

Client::~Client()
{}