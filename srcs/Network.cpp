#include "Network.hpp"
#include "sys/epoll.h"
#include "utils.hpp"
#include "sys/socket.h"
#include <netinet/in.h>

void Network::instanceEpoll()
{
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1)
        throw(std::logic_error(messageError("instance_epoll>epoll_create")));
}


void Network::addingServers()
{
    for(size_t i = 0; i < server_vec_.size(); i++)
    {
        int fd = server_vec_[i].getFd();
        
        ev_.data.fd = fd;
        ev_.events = EPOLLIN;
        
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev_) == -1)
             throw(std::logic_error(messageError("addingServers>epoll_ctl")));
    }
}



void Network::manageNetwork()
{
    while (true)
    {
        int nb_events = epoll_wait(epoll_fd_, events_, MAX_EVENTS, -1 );
        for (int i = 0; i < nb_events; i++)
        {
            for (size_t j = 0; j < server_vec_.size(); j++)
            {
                if (events_[i].data.fd == server_vec_[j].getFd())
                {
                   acceptNewClient(server_vec_[j]);
                   displayClients();
                }
                else if (events_[i].events & EPOLLIN && clientIsInsideServer(events_[i].data.fd, server_vec_[j]))
                {
                    getClientRequest(server_vec_[j], events_[i].data.fd);
                }
            }
        }
    }
}

void Network::getClientRequest(Server const& server, int client_fd) const
{
    (void)server;

    int bytes;
    char buffer[100000] = {};
    bytes = recv(client_fd, buffer, sizeof(buffer), 0);
    std::cout << server.getPort() << ": " << buffer << std::endl;
}

void Network::acceptNewClient(Server const& server)
{
    while (true)
    {
        sockaddr_in client_addr = {};

        socklen_t size = sizeof(client_addr);

        int client_fd = accept(server.getFd(), reinterpret_cast<sockaddr *>(&client_addr), &size);
        
        if (client_fd == -1)
            break;
        
        ev_.data.fd = client_fd;
        
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &ev_) == -1)
            throw(std::logic_error(messageError("acceptNewClient>epoll_ctl")));
        
        Client client(client_fd, server);
        client_vec_.push_back(client);
    }
}

void Network::displayClients() const
{
    std::cout << "----------------------------" << std::endl;
    for (size_t i = 0; i < client_vec_.size(); i++)
    {
        Client tmp;

        tmp = client_vec_[i];

        std::cout << "client_fd: "<< tmp.getClient() << std::endl;
        std::cout << "server_fd: " << tmp.getServer().getFd() << std::endl;
        std::cout << "----------------------------" << std::endl;
    }
    std::cout << std::endl;
}

int Network::getServerFdFromClient(int client_fd) const
{
    for(size_t i = 0; i < client_vec_.size(); i++)
    {
        if (client_vec_[i].getClient() == client_fd)
        {
            return (client_vec_[i].getServer().getFd());
        }
    }
    return -1;
}

int Network::clientIsInsideServer(int client_fd, Server &server) const
{
    if (getServerFdFromClient(client_fd) == server.getFd())
        return (1);
    return (0);
}

void Network::setClients(std::vector<Client> const& clients)
{
    client_vec_ = clients;
}

void Network::setServers(std::vector<Server> const& servers)
{
    server_vec_ = servers;
}

std::vector<Server> const&  Network::getServers() const
{
    return (server_vec_);
}


std::vector<Client> const&  Network::getClients() const 
{
    return (client_vec_);
}

Network::Network():
    epoll_fd_(-1), 
    server_vec_(0), 
    client_vec_(0)
    {}

Network::Network(std::vector<Server> &servers, std::vector<Client> &clients):
    server_vec_(servers), 
    client_vec_(clients)
    {}

Network::Network(Network const& to_copy):
    epoll_fd_(to_copy.epoll_fd_),
    server_vec_(to_copy.server_vec_), 
    client_vec_(to_copy.client_vec_)
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

Network::~Network()
{}


