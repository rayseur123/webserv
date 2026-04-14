#include "Client.hpp"
#include <iostream>

int Client::getClient() const
{
    return (client_fd_);
}

Server const& Client::getServer() const
{
    return (server_);
}

Client::Client(): 
    client_fd_(-1)
    {}

Client::Client(int client, Server const& server): 
    server_(server),
    client_fd_(client) 
    {}

Client::Client(Client const& to_copy): 
    server_(to_copy.server_),
    client_fd_(to_copy.client_fd_)
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

std::ostream& operator<<(std::ostream& os, Client const& client)
{
    os << "client_fd: " << client.getClient() << std::endl;
    os << "server_fd: " << client.getServer().getFd() << std::endl;
    return (os);
}