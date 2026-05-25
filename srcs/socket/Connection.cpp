#include <csignal>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "epoll/EpollManager.hpp"
#include "epoll/signal.hpp"
#include "http/Cgi.hpp"
#include "http/Code.hpp"
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
Connection::handleCGI(Request const& request_http, std::string& response_str,
					  std::string& path, Location const& location)
{
	Cgi request_cgi;

	try
	{
		request_cgi.buildEnv(request_http, server_, getClientAddr(), path);
		request_cgi.startProgram(request_http, *this, path, location);
	}
	catch (Code& e)
	{
		response_str = buildErrorResponse(e.getCode(), server_, "HTTP/1.1");
		return (sendMsg(response_str));
	}
	return (false);
}

void
Connection::handleHTTP(Request const& request, std::string& response_str,
					   std::string const& path, Location const& location)
{
	int type = request.getMethod().getType();
	if (type == GET)
	{
		ResponseGet response(request);
		response_str = response.buildResponse(location, server_, path);
	}
	else if (type == POST)
	{
		ResponsePost response(request);
		response_str = response.buildResponse(location, server_, path);
	}
	else if (type == DELETE)
	{
		ResponseDelete response(request);
		response_str = response.buildResponse(location, server_, path);
	}
	if (Signal::signal == 1)
		throw(SIGINT);
}

bool
isCGI(std::string const& uri)
{
	size_t		pos = 0;
	std::string check;

	pos = uri.find(".py");
	if (pos != std::string::npos)
	{
		check = uri.substr(pos);
		if (check.length() == 3)
			return true;
	}
	pos = uri.find(".php");
	if (pos != std::string::npos)
	{
		return true;
	}

	return false;
}

int
Connection::handleConnectionRequest()
{
	size_t bytes = 0;
	char   buffer[10000] = {};

	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	if (bytes == 0 && isCGI(parsing_request_.getRequest().getUri().getTarget()))
		return (1);
	if (bytes == 0)
		return (0);

	// Parsing client request
	std::string tmp(buffer, bytes);
	parsing_request_.fillBuffer(tmp);

	if (Signal::signal == 1)
		throw(SIGINT);

	if (parsing_request_.getStep() != FINISH)
		return 0;

	Request request = parsing_request_.getRequest();
	request.setCode(parsing_request_.getCode());

	Location	location = getGoodLocation(server_.getLocations(), request);
	std::string path = location.buildPath(request);

	std::string response_str;
	// Check error inside the parsing
	if (request.getCode() != 0)
	{
		response_str = buildErrorResponse(request.getCode(), server_,
										  request.getVersion().toString());
		std::cout << response_str << std::endl;
		return (sendMsg(response_str));
	}

	if (isCGI(request.getUri().getTarget()))
	{
		return (handleCGI(request, response_str, path, location));
	}
	handleHTTP(request, response_str, path, location);

	return (sendMsg(response_str));
}

bool
Connection::sendMsg(std::string const& msg)
{
	send(fd_, msg.c_str(), msg.size(), 0);

	if (Signal::signal == 1)
		throw(SIGINT);

	return (true);
}

int
Connection::handleEvent(EpollManager& manager, uint32_t events)
{
	// Adding the size_body_max inside the parsing_request;
	events_ = events;
	parsing_request_.setMaxBodyLength(server_.getMaxClientRequestBody());
	(void) manager;
	if ((events & (EPOLLERR | EPOLLRDHUP | EPOLLHUP)) != 0)
		return (1);
	if ((events & (EPOLLIN | EPOLLPRI)) != 0)
		return (handleConnectionRequest());
	return (0);
}

EpollManager&
Connection::getManager() const
{
	return manager_;
}

Listener const&
Connection::getServer() const
{
	return (server_);
}

std::string const&
Connection::getClientAddr() const
{
	return addr_client_;
}

Connection::Connection(int fd, Listener& server, std::string const& addr,
					   EpollManager& manager) :
	ASocket(fd), server_(server), manager_(manager), events_(),
	addr_client_(addr)
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
