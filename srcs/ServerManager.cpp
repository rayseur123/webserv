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

void ServerManager::eventLoop()
{
    std::vector<Server>::iterator   it;

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
                else if (events_[i].events && connectionIsInsideServer(events_[i].data.fd, *it))
                {
                    Connection connection = getConnectionByFd(events_[i].data.fd);
                    if (handleEventsConnection(*it, connection, events_[i].events))
                    {
                        std::cout << "Connection on fd " << events_[i].data.fd << " has been disconected." << std::endl;
                        connection_vec_.erase(std::find(connection_vec_.begin(), connection_vec_.end(), connection));
                        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, events_[i].data.fd, NULL);
                    }
                }
            }
        }
    }
}

Connection const&   ServerManager::getConnectionByFd(int connection_fd) const
{
    std::vector<Connection>::const_iterator it = connection_vec_.begin();

    while (it != connection_vec_.end())
    {
        if (it->getFd() == connection_fd)
            break ;
        it++;
    }
    return (*it);
}

int    ServerManager::handleEventsServer(Server const& server, uint32_t events)
{
    if (events & (EPOLLERR | EPOLLRDHUP))
    {
        close(server.getFd());
        return (1);
    }
    else if (events & EPOLLIN)
        acceptNewConnection(server);
    return (0);
}

int ServerManager::handleEventsConnection(Server const& server, Connection const& connection, uint32_t events)
{
    if (events & (EPOLLERR | EPOLLRDHUP))
    {
        close(connection.getFd());
        return (1);
    }
    if (events & (EPOLLIN | EPOLLPRI))
        return (getConnectionRequest(server, connection.getFd()));
    // rajouter un if respond quand pour le http plus tard.
    return (0);
}

int ServerManager::getConnectionRequest(Server const& server, int connection_fd) const
{
    (void)server;
    int bytes;
    char buffer[100000] = {};

    bytes = recv(connection_fd, buffer, sizeof(buffer), 0);
    if (!bytes)
        return (1);
    std::cout << server.getPort() << ": " << buffer << std::endl;
    return (0);
}

void ServerManager::acceptNewConnection(Server const& server)
{
    epoll_event ev;

    while (true)
    {
        sockaddr_in connection_addr = {};
        socklen_t size = sizeof(connection_addr);

        int connection_fd = accept(server.getFd(), reinterpret_cast<sockaddr *>(&connection_addr), &size);
        
        if (connection_fd == -1)
            break;
        
        ev.data.fd = connection_fd;
        ev.events = EPOLLIN; // Rajouter les events client
        
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, connection_fd, &ev) == -1)
            throw std::logic_error(messageError("acceptNewClient>epoll_ctl"));
        
        Connection connection(connection_fd, server);
        connection_vec_.push_back(connection);
    }
}

int ServerManager::findServerFdFromConnection(int connection_fd) const
{
    std::vector<Connection>::const_iterator it;

    for (it = connection_vec_.begin(); it != connection_vec_.end(); ++it)
    {
        if (it->getFd() == connection_fd)
            return (it->getServer().getFd());
    }
    return (-1);
}

int ServerManager::connectionIsInsideServer(int connection_fd, Server &server) const
{
    if (findServerFdFromConnection(connection_fd) == server.getFd())
        return (1);
    return (0);
}

void ServerManager::setConnectionsVec(std::vector<Connection> const& connection_vec)
{
    connection_vec_ = connection_vec;
}

void ServerManager::setServersVec(std::vector<Server> const& servers)
{
    server_vec_ = servers;
}

std::vector<Server> const&  ServerManager::getServersVec() const
{
    return (server_vec_);
}


std::vector<Connection> const&  ServerManager::getConnectionVec() const 
{
    return (connection_vec_);
}

ServerManager::ServerManager():
    epoll_fd_(-1), 
    server_vec_(0), 
    connection_vec_(0)
    {}

ServerManager::ServerManager(std::vector<Server> &servers, std::vector<Connection> &clients_vec):
    server_vec_(servers), 
    connection_vec_(clients_vec)
{

    std::vector<Server>::iterator it;

    for (it = server_vec_.begin(); it != server_vec_.end(); ++it)
		it->createSocket();

    instanceEpoll();
    addingServers();
    eventLoop();
}

ServerManager::ServerManager(ServerManager const& to_copy):
    epoll_fd_(to_copy.epoll_fd_),
    server_vec_(to_copy.server_vec_), 
    connection_vec_(to_copy.connection_vec_)
    {} 

ServerManager const& ServerManager::operator=(ServerManager const& to_copy)
{
    if (this == &to_copy)
        return *this;
    epoll_fd_ = to_copy.epoll_fd_;    
    server_vec_ = to_copy.server_vec_;
    connection_vec_ = to_copy.connection_vec_;
    return *this;
}    

ServerManager::~ServerManager()
{}


