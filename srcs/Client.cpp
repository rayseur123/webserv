#include "Client.hpp"
#include <iostream>

int const& Client::getClient() const
{
    return (client_fd_);
}

Server const& Client::getServer() const
{
    return (server_);
}

Client::Client()
: client_fd_(-1)
{}

Client::Client(int client, Server const& server)
: client_fd_(client), server_(server)
{}

Client::Client(Client const& to_copy)
: client_fd_(to_copy.client_fd_), server_(to_copy.server_)
{}

Client const& Client::operator=(Client const& to_copy)
{
    if (this == &to_copy)
        return (*this);
    client_fd_ = to_copy.client_fd_;
    server_ = to_copy.server_;
    return (*this);
}

Client::~Client()
{}