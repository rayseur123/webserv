#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "ASocket.hpp"
#include <fstream>

class Listener;
class EpollManager;

class	Connection : public ASocket
{
	private:
		Listener const&	server_;
	public:
        int                 getConnectionRequest() const;

        virtual int		    handleEvent(EpollManager& manager, int events);
		Listener const&     getServer() const;

        Connection(int fd, Listener& server);
        Connection(Connection const& to_copy);
        Connection const&   operator=(Connection const& to_copy);
        ~Connection();
};

std::ostream& operator<<(std::ostream& os, Connection const& connection);

#endif
