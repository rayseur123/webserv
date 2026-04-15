#include "ServerManager.hpp"
#include "sys/epoll.h"
#include "utils.hpp"
#include "sys/socket.h"
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>

void ServerManager::instanceEpoll()
{
    epoll_fd_ = epoll_create(2);
    if (epoll_fd_ == -1)
        throw std::logic_error(messageError("instance_epoll>epoll_create"));
}

void ServerManager::addingServers()
{   
    std::vector<Server>::iterator   it;
    epoll_event                     ev;
    
    for (it = server_vec_.begin(); it != server_vec_.end(); ++it)
    {
        int fd = it->getFd();
        
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLERR | EPOLLRDHUP;
        
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1)
             throw std::logic_error(messageError("addingServers>epoll_ctl"));
    }
}

void ServerManager::manageServers()
{
    std::vector<Server>::iterator   it;
    uint32_t                        event_check;

    event_check = EPOLLIN; // rajouter les events ici
    while (true)
    {
        int nb_events = epoll_wait(epoll_fd_, events_, MAX_EVENTS, -1 );
        for (int i = 0; i < nb_events; ++i)
        {
            for (it = server_vec_.begin(); it != server_vec_.end(); ++it)
            {
                if (events_[i].data.fd == it->getFd())
                {
                    if (handleEventsServer(*it, events_[i].events))
                    {
                        std::cout << "Server on fd " << it->getFd() << " has been shut down." << std::endl;
                        server_vec_.erase(it);
                        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, it->getFd(), NULL);
                    }
                }
                else if (events_[i].events && clientIsInsideServer(events_[i].data.fd, *it))
                {
                    Client client = getClientByFd(events_[i].data.fd);
                    if (handleEventsClient(*it, client, events_[i].events))
                    {
                        std::cout << "Client on fd " << events_[i].data.fd << " has been disconected." << std::endl;
                        client_vec_.erase(std::find(client_vec_.begin(), client_vec_.end(), client));
                        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, events_[i].data.fd, NULL);
                    }
                }
            }
        }
    }
}

Client const&   ServerManager::getClientByFd(int client_fd) const
{
    std::vector<Client>::const_iterator it = client_vec_.begin();

    while (it != client_vec_.end())
    {
        if (it->getFd() == client_fd)
            break ;
        it++;
    }
    return (*it);
}

int    ServerManager::handleEventsServer(Server const& server, uint32_t events)
{
    if (events & (EPOLLERR | EPOLLHUP))
    {
        close(server.getFd());
        return (1);
    }
    else if (events & EPOLLIN)
        acceptNewClient(server);
    return (0);
}

int ServerManager::handleEventsClient(Server const& server, Client const& client, uint32_t events)
{
    if (events & (EPOLLERR | EPOLLHUP))
    {
        close(client.getFd());
        return (1);
    }
    if (events & (EPOLLIN | EPOLLPRI))
        return (getClientRequest(server, client.getFd()));
    // rajouter un if respond quand pour le http plus tard.
    return (0);
}

int ServerManager::getClientRequest(Server const& server, int client_fd) const
{
    (void)server;

    int bytes;
    char buffer[100000] = {};

    bytes = recv(client_fd, buffer, sizeof(buffer), 0);
    if (!bytes)
        return (1);
    std::cout << server.getPort() << ": " << buffer << std::endl;
    return (0);
}

void ServerManager::acceptNewClient(Server const& server)
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

int ServerManager::findServerFdFromClient(int client_fd) const
{
    std::vector<Client>::const_iterator it;

    for (it = client_vec_.begin(); it != client_vec_.end(); ++it)
    {
        if (it->getFd() == client_fd)
            return (it->getServer().getFd());
    }
    return (-1);
}

int ServerManager::clientIsInsideServer(int client_fd, Server &server) const
{
    if (findServerFdFromClient(client_fd) == server.getFd())
        return (1);
    return (0);
}

void ServerManager::setClients(std::vector<Client> const& clients)
{
    client_vec_ = clients;
}

void ServerManager::setServers(std::vector<Server> const& servers)
{
    server_vec_ = servers;
}

std::vector<Server> const&  ServerManager::getServers() const
{
    return (server_vec_);
}


std::vector<Client> const&  ServerManager::getClients() const 
{
    return (client_vec_);
}

ServerManager::ServerManager():
    epoll_fd_(-1), 
    server_vec_(0), 
    client_vec_(0)
    {}

ServerManager::ServerManager(std::vector<Server> &servers, std::vector<Client> &clients):
    server_vec_(servers), 
    client_vec_(clients)
{

    std::vector<Server>::iterator it;

    for (it = server_vec_.begin(); it != server_vec_.end(); ++it)
		it->createSocket();

    instanceEpoll();
    addingServers();
    manageServers();
}

ServerManager::ServerManager(ServerManager const& to_copy):
    epoll_fd_(to_copy.epoll_fd_),
    server_vec_(to_copy.server_vec_), 
    client_vec_(to_copy.client_vec_)
    {} 

ServerManager const& ServerManager::operator=(ServerManager const& to_copy)
{
    if (this == &to_copy)
        return *this;
    epoll_fd_ = to_copy.epoll_fd_;    
    server_vec_ = to_copy.server_vec_;
    client_vec_ = to_copy.client_vec_;
    return *this;
}    

ServerManager::~ServerManager()
{}


