#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <Client.hpp>
#include <Server.hpp>
#include <sys/epoll.h>

#define MAX_EVENTS 100

class Network
{
    private:
        
        int                 epoll_fd_;
        
        epoll_event         events_[MAX_EVENTS];

        std::vector<Server> server_vec_;
        std::vector<Client> client_vec_;

    public:

        void                        instanceEpoll();
        void                        addingServers();
        void                        manageNetwork();
        void                        acceptNewClient(Server const& server_fd);
        void                        getClientRequest(Server const& server_fd, int client_fd) const;
        
        int                         findServerFdFromClient(int client_fd) const;
        int                         clientIsInsideServer(int client_fd, Server &server) const;

        void                        setServers(std::vector<Server> const& servers);
        void                        setClients(std::vector<Client> const& clients);
        std::vector<Server> const&  getServers() const;
        std::vector<Client> const&  getClients() const;

        Network();
        Network(std::vector<Server> &servers, std::vector<Client> &clients);
        Network(Network const& to_copy);
        Network const& operator=(Network const& to_copy);
        ~Network();
};

#endif