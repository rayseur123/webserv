#include "Client.hpp"
#include <iostream>

int Connection::getFd() const
{
    return (fd_);
}

Server const& Connection::getServer() const
{
    return (*server_);
}

Connection::Connection(): 
    server_(NULL),
    fd_(-1)
    {}

Connection::Connection(int client, Server const& server): 
    server_(&server),
    fd_(client) 
    {}

Connection::Connection(Connection const& to_copy): 
    server_(to_copy.server_),
    fd_(to_copy.fd_)
    {}

Connection const& Connection::operator=(Connection const& to_copy)
{
    if (this == &to_copy)
        return (*this);
    fd_ = to_copy.fd_;
    server_ = to_copy.server_;
    return (*this);
}

bool    Connection::operator==(Connection const& to_comp)
{
    return (to_comp.fd_ == fd_);
}

Connection::~Connection()
{}

std::ostream& operator<<(std::ostream& os, Connection const& client)
{
    os << "client_fd: " << client.getFd() << std::endl;
    os << "server_fd: " << client.getServer().getFd() << std::endl;
    return (os);
}