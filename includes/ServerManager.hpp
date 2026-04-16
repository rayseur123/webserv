#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <Connection.hpp>
#include <Server.hpp>
#include <sys/epoll.h>
#include <map>

#define MAX_EVENTS 100

class ServerManager
{
    private:
        
        int                 epoll_fd_;
        
        epoll_event         events_[MAX_EVENTS];

        std::vector<Server> server_vec_;
        std::vector<Connection> connection_vec_;

    public:

        void                        instanceEpoll();
        void                        addingServers();
        void                        eventLoop();
        void                        acceptNewConnection(Server const& server_fd);
        int                         getConnectionRequest(Server const& server_fd, int connection_fd) const;
        Connection const&           getConnectionByFd(int connection_fd) const;
        int                         handleEventsServer(Server const& server, uint32_t events);
        int                         handleEventsConnection(Server const& server, Connection const& connection, uint32_t events);

        int                         findServerFdFromConnection(int connection_fd) const;
        int                         connectionIsInsideServer(int connection_fd, Server &server) const;

        void                        setServersVec(std::vector<Server> const& servers_vec);
        void                        setConnectionsVec(std::vector<Connection> const& connections_vec);
        std::vector<Server> const&  getServersVec() const;
        std::vector<Connection> const&  getConnectionVec() const;


        ServerManager();
        ServerManager(std::vector<Server> &servers, std::vector<Connection> &connections_vec);
        ServerManager(ServerManager const& to_copy);
        ServerManager const& operator=(ServerManager const& to_copy);
        ~ServerManager();
};

#endif