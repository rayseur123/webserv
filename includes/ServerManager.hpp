#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <Connection.hpp>
#include <Server.hpp>
#include <sys/epoll.h>
#include <map>


#define EVENTS_CONNECTION EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI
#define EVENTS_SERVER EPOLLIN | EPOLLERR | EPOLLRDHUP
#define MAX_EVENTS 100

class ServerManager
{
    private:
        
        int                     epoll_fd_;
        
        epoll_event             events_[MAX_EVENTS];

        std::map<int, Server>       server_map_;
        std::map<int, Connection>   connection_map_;
    public:

        void                            instanceEpoll();
        void                            registerServersToEpoll();
        void                            eventLoop();
        void                            acceptNewConnection(int fd);
        int                             getConnectionRequest(Server const& server_fd, int connection_fd) const;
        int                             handleEventsServer(int fd, uint32_t events);
        int                             handleEventsConnection(Connection const& connection, uint32_t events);

        ServerManager();
        ServerManager(std::vector<Server> &servers);
        ServerManager(ServerManager const& to_copy);
        ServerManager const& operator=(ServerManager const& to_copy);
        ~ServerManager();
};

#endif