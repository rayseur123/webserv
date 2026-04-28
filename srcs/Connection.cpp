#include "Connection.hpp"
#include "Listener.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <Request.hpp>

int Connection::handleConnectionRequest()
{
    int bytes;
    char buffer[10000]= {};

    bytes = recv(fd_, buffer, sizeof(buffer), 0);
    if (!bytes)
		return (1);
		
	std::string tmp(buffer, bytes);
	// std::string tmp;

	std::cout << tmp << std::endl << std::endl << std::endl;

	// tmp = "GET /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\nA\r\nHelloWorld\r\n0\r\n\r\n";
	parsing_request_.fillBuffer(tmp);

	if (parsing_request_.getStep() != FINISH)
		return 0;
	
	std::cout << parsing_request_.getRequest();
    // std::cout << server_.getPort() << ": " << buffer << std::endl;
    return (0);
}

int		Connection::handleEvent(EpollManager& manager, int events)
{
	(void)manager;
	if (events & (EPOLLERR | EPOLLRDHUP))
		return (1);
	if (events & (EPOLLIN | EPOLLPRI))
		return (handleConnectionRequest());
	// rajouter un if respond quand pour le http plus tard.
	return (0);
}

Listener const&   Connection::getServer() const
{
	return (server_);
}

Connection::Connection(int fd, Listener& server)
: 	ASocket(fd),
	server_(server)
{}

Connection::Connection(Connection const& to_copy)
:server_(to_copy.server_)
{}

Connection const&   Connection::operator=(Connection const& to_copy)
{
	(void)to_copy;
	return (*this);
}

Connection::~Connection()
{}

std::ostream& operator<<(std::ostream& os, Connection const& connection)
{
	os << "connection_fd: " << connection.getFd() << std::endl;
    os << "server_fd: " << connection.getServer().getFd() << std::endl;
    return (os);
}