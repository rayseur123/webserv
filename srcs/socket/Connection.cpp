#include <csignal>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "epoll/signal.hpp"
#include "http/httpStatus.hpp"
#include "http/parsing/Method.hpp"
#include "http/parsing/ParsingRequest.hpp"
#include "http/parsing/Request.hpp"
#include "http/ResponseDelete.hpp"
#include "http/ResponseGet.hpp"
#include "http/ResponsePost.hpp"
#include "socket/Connection.hpp"
#include "socket/Listener.hpp"
#include "utils/utils.hpp"

bool
Connection::bodyLengthValid()
{
	return (parsing_request_.getRequest().getBody().getLength() <=
			server_.getMaxClientRequestBody());
}

int
Connection::handleConnectionRequest()
{
	size_t bytes = 0;
	char   buffer[10000] = {};

	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	if (bytes == 0)
		return (1);

	std::string tmp(buffer, bytes);
	parsing_request_.fillBuffer(tmp);

	if (Signal::signal == 1)
		throw(SIGINT);

	if (parsing_request_.getStep() != FINISH)
		return 0;

	Request request = parsing_request_.getRequest();

	if (!bodyLengthValid())
		request.setCode(HTTP_PAYLOAD_TOO_LARGE);
	else
		request.setCode(parsing_request_.getCode());

	std::string response_str;

	if (request.getCode() != 0)
		response_str = buildErrorResponse(request.getCode());
	else
	{
		int type = request.getMethod().getType();
		if (type == GET)
		{
			ResponseGet response(request);
			response_str = response.buildResponse(server_.getLocations());
		}
		else if (type == POST)
		{
			ResponsePost response(request);
			response_str = response.buildResponse(server_.getLocations());
		}
		else if (type == DELETE)
		{
			ResponseDelete response(request);
			response_str = response.buildResponse(server_.getLocations());
		}
		if (Signal::signal == 1)
			throw(SIGINT);
	}

	send(fd_, response_str.c_str(), response_str.size(), 0);

	if (Signal::signal == 1)
		throw(SIGINT);

	parsing_request_.resetParsingAndRequest();
	return (1);
}

int
Connection::handleEvent(EpollManager& manager, uint32_t events)
{
	(void) manager;
	if ((events & (EPOLLERR | EPOLLRDHUP)) != 0)
		return (1);
	if ((events & (EPOLLIN | EPOLLPRI)) != 0)
		return (handleConnectionRequest());
	return (0);
}

Listener const&
Connection::getServer() const
{
	return (server_);
}

Connection::Connection(int fd, Listener& server) : ASocket(fd), server_(server)
{}

Connection::~Connection()
{}

std::ostream&
operator<<(std::ostream& os, Connection const& connection)
{
	os << "connection_fd: " << connection.getFd() << '\n';
	os << "server_fd: " << connection.getServer().getFd() << '\n';
	return (os);
}
