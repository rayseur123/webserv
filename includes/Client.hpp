#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>

class Client
{
    private:
        
        int client_fd_;
        int server_fd_;
    
    public:

        int const& getClient() const;
        int const& getServer() const;
        
        Client();
        Client(int client, int server);
        Client(Client const& to_copy);
        Client const& operator=(Client const& to_copy);
        ~Client();
    };
    
int findServerByClient(std::vector<Client> client_vec, int client_fd);

#endif