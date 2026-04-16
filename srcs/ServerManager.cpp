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

void ServerManager::registerServersToEpoll()
{   
    std::map<int, Server>::iterator it;
    epoll_event                     ev;
    
    for (it = server_map_.begin(); it != server_map_.end(); it++)
    {
        int fd = it->second.getFd();
        ev.data.fd = fd;
        ev.events = EVENTS_SERVER;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1)
             throw std::logic_error(messageError("registerServersToEpoll>epoll_ctl"));
    }
}

void ServerManager::eventLoop()
{
    while (true)
    {
        int nb_events = epoll_wait(epoll_fd_, events_, MAX_EVENTS, -1 );
        for (int i = 0; i < nb_events; ++i)
        {
            int fd = events_[i].data.fd;
            std::map<int, Server>::iterator it = server_map_.find(fd);
            if (it != server_map_.end())
            {
                if (handleEventsServer(fd, events_[i].events))
                {
                    std::cout << "Server on fd " << fd << " has been shut down." << std::endl;
                    server_map_.erase(it);
                    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
                }
            }
            else
            {
                if (handleEventsConnection(connection_map_[fd], events_[i].events))
                {
                    std::cout << "Connection on fd " << fd << " has been disconected." << std::endl;
                    connection_map_.erase(connection_map_.find(fd));
                    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
                }
            }
        }
    }
}

int    ServerManager::handleEventsServer(int fd, uint32_t events)
{
    if (events & (EPOLLERR | EPOLLRDHUP))
    {
        close(fd);
        return (1);
    }
    else if (events & EPOLLIN)
        acceptNewConnection(fd);
    return (0);
}

int ServerManager::handleEventsConnection(Connection const& connection, uint32_t events)
{
    if (events & (EPOLLERR | EPOLLRDHUP))
    {
        close(connection.getFd());
        return (1);
    }
    if (events & (EPOLLIN | EPOLLPRI))
        return (getConnectionRequest(connection.getServer(), connection.getFd()));
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

void ServerManager::acceptNewConnection(int fd)
{
    epoll_event ev;

    while (true)
    {
        sockaddr_in connection_addr = {};
        socklen_t size = sizeof(connection_addr);

        int connection_fd = accept(fd, reinterpret_cast<sockaddr *>(&connection_addr), &size);
        
        if (connection_fd == -1)
            break;
        
        ev.data.fd = connection_fd;
        ev.events = EVENTS_CONNECTION;
        
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, connection_fd, &ev) == -1)
            throw std::logic_error(messageError("acceptNewClient>epoll_ctl"));
        
        Connection connection(connection_fd, server_map_[fd]);
        connection_map_.insert(std::make_pair(connection_fd, connection));
    }
}

ServerManager::ServerManager():
    epoll_fd_(-1)
{}

ServerManager::ServerManager(std::vector<Server> &server_vec)
{
    std::vector<Server>::iterator it;

    for (it = server_vec.begin(); it != server_vec.end(); it++)
        server_map_.insert(std::make_pair(it->createSocket(), *it));

    instanceEpoll();
    registerServersToEpoll();
    eventLoop();
}

ServerManager::ServerManager(ServerManager const& to_copy):
    epoll_fd_(to_copy.epoll_fd_),
    server_map_(to_copy.server_map_)
{} 

ServerManager const& ServerManager::operator=(ServerManager const& to_copy)
{
    if (this == &to_copy)
        return *this;
    epoll_fd_ = to_copy.epoll_fd_;  
    server_map_ = to_copy.server_map_;  
    return *this;
}    

ServerManager::~ServerManager()
{}


