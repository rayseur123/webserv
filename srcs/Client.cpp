#include "Client.hpp"
#include <iostream>

int Client::getFd() const
{
    return (fd_);
}

Server const& Client::getServer() const
{
    return (*server_);
}

Client::Client(): 
    server_(NULL),
    fd_(-1)
    {}

Client::Client(int client, Server const& server): 
    server_(&server),
    fd_(client) 
    {}

Client::Client(Client const& to_copy): 
    server_(to_copy.server_),
    fd_(to_copy.fd_)
    {}

Client const& Client::operator=(Client const& to_copy)
{
    if (this == &to_copy)
        return (*this);
    fd_ = to_copy.fd_;
    server_ = to_copy.server_;
    return (*this);
}

bool    Client::operator==(Client const& to_comp)
{
    return (to_comp.fd_ == fd_);
}

Client::~Client()
{}

std::ostream& operator<<(std::ostream& os, Client const& client)
{
    os << "client_fd: " << client.getFd() << std::endl;
    os << "server_fd: " << client.getServer().getFd() << std::endl;
    return (os);
}