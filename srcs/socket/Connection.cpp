#include "socket/Connection.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "http/parsing/Request.hpp"
#include "http/ResponseGet.hpp"
#include "socket/Listener.hpp"

int
Connection::handleConnectionRequest()
{
	int	 bytes;
	char buffer[10000] = {};

	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	if (!bytes)
		return (1);

	std::string tmp(buffer, bytes);
	// std::string tmp;

	tmp = "GET /test/ HTTP/1.1\r\nHost: exemple.fr\r\nContent-Type: "
		  "\r\nContent-Length: 27\r\n\r\nfield1=value1&field2=value2\r\n";
	parsing_request_.fillBuffer(tmp);

	if (parsing_request_.getStep() != FINISH)
		return 0;
	std::cout << parsing_request_.getRequest() << std::endl;
	// ResponseGet	response(request);

	// std::cout << "uri : " << request.getUri();
	// response.buildResponseStr(server_.getLocations());
	return (0);
}

int
Connection::handleEvent(EpollManager& manager, uint32_t events)
{
	(void) manager;
	if (events & (EPOLLERR | EPOLLRDHUP))
		return (1);
	if (events & (EPOLLIN | EPOLLPRI))
		return (handleConnectionRequest());
	// rajouter un if respond quand pour le http plus tard.
	return (0);
}

Listener const&
Connection::getServer() const
{
	return (server_);
}

Connection::Connection(int fd, Listener& server) : ASocket(fd), server_(server)
{}

Connection::Connection(Connection const& to_copy) : server_(to_copy.server_)
{}

Connection&
Connection::operator=(Connection const& to_copy)
{
	(void) to_copy;
	return (*this);
}

Connection::~Connection()
{}

std::ostream&
operator<<(std::ostream& os, Connection const& connection)
{
	os << "connection_fd: " << connection.getFd() << std::endl;
	os << "server_fd: " << connection.getServer().getFd() << std::endl;
	return (os);
}
