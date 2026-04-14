#include "Network.hpp"
#include "sys/epoll.h"
#include "utils.hpp"
#include "sys/socket.h"
#include <netinet/in.h>

void Network::instanceEpoll()
{
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1)
        throw std::logic_error(messageError("instance_epoll>epoll_create"));
}


void Network::addingServers()
{   
    std::vector<Server>::iterator   it;
    epoll_event                     ev;
    
    for (it = server_vec_.begin(); it != server_vec_.end(); ++it)
    {
        int fd = (*it).getFd();
        
        ev.data.fd = fd;
        ev.events = EPOLLIN;
        
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1)
             throw std::logic_error(messageError("addingServers>epoll_ctl"));
    }
}



void Network::manageNetwork()
{
    std::vector<Server>::iterator it;

    while (true)
    {
        int nb_events = epoll_wait(epoll_fd_, events_, MAX_EVENTS, -1 );
        for (int i = 0; i < nb_events; i++)
        {
            for (it = server_vec_.begin(); it != server_vec_.end(); ++it)
            {
                if (events_[i].data.fd == (*it).getFd())
                    acceptNewClient(*it);
                else if (events_[i].events & EPOLLIN && clientIsInsideServer(events_[i].data.fd, *it))
                    getClientRequest(*it, events_[i].data.fd);
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
    epoll_event ev;

    while (true)
    {
        sockaddr_in client_addr = {};
        socklen_t size = sizeof(client_addr);

        int client_fd = accept(server.getFd(), reinterpret_cast<sockaddr *>(&client_addr), &size);
        
        if (client_fd == -1)
            break;
        
        ev.data.fd = client_fd;
        ev.events = EPOLLIN;
        
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &ev) == -1)
            throw std::logic_error(messageError("acceptNewClient>epoll_ctl"));
        
        Client client(client_fd, server);
        client_vec_.push_back(client);
    }
}

int Network::findServerFdFromClient(int client_fd)
{
    std::vector<Client>::iterator it;

    for (it = client_vec_.begin(); it != client_vec_.end(); it++)
    {
        if ((*it).getClient() == client_fd)
            return ((*it).getServer().getFd());
    }
    return (-1);
}

int Network::clientIsInsideServer(int client_fd, Server &server)
{
    if (findServerFdFromClient(client_fd) == server.getFd())
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
{

    std::vector<Server>::iterator it;

    for (it = server_vec_.begin(); it != server_vec_.end(); it++)
		(*it).createSocket();

    instanceEpoll();
    addingServers();
    manageNetwork();
}

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


