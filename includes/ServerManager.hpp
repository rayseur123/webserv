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
        std::vector<Connection> client_vec_;

    public:

        void                        instanceEpoll();
        void                        addingServers();
        void                        eventLoop();
        void                        acceptNewClient(Server const& server_fd);
        int                         getClientRequest(Server const& server_fd, int client_fd) const;
        Connection const&           getClientByFd(int client_fd) const;
        int                         handleEventsServer(Server const& server, uint32_t events);
        int                         handleEventsClient(Server const& server, Connection const& client, uint32_t events);

        int                         findServerFdFromClient(int client_fd) const;
        int                         clientIsInsideServer(int client_fd, Server &server) const;

        void                        setServers(std::vector<Server> const& servers);
        void                        setClients(std::vector<Connection> const& clients);
        std::vector<Server> const&  getServers() const;
        std::vector<Connection> const&  getClients() const;


        ServerManager();
        ServerManager(std::vector<Server> &servers, std::vector<Connection> &clients);
        ServerManager(ServerManager const& to_copy);
        ServerManager const& operator=(ServerManager const& to_copy);
        ~ServerManager();
};

#endif