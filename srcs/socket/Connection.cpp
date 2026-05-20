#include <csignal>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "epoll/EpollManager.hpp"
#include "epoll/signal.hpp"
#include "http/Cgi.hpp"
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

void
Connection::handleCGI(Request const& request, std::string& response_str,
					  std::string const& path, Location const& location)
{
	Cgi response;

	response.buildEnv(request, server_, getClientAddr());
	response.startProgram(request, *this, path, location);

	response_str = "final response from cgi \n";
}

void
Connection::handleHTTP(Request const& request, std::string& response_str,
					   std::string const& path, Location const& location)
{
	if (request.getCode() != 0)
		response_str = buildErrorResponse(request.getCode(), server_,
										  request.getVersion().toString());
	else
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
}

// Change in the future directly check by the folder and the extension
// need to see with nicolas and what is cgi_pass
bool
isCGI(std::string const& uri)
{
	size_t pos = 0;

	pos = uri.find(".py");

	return (pos != std::string::npos);
}

int
Connection::handleConnectionRequest()
{
	size_t bytes = 0;
	char   buffer[10000] = {};

	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	if (bytes == 0)
		return (1);

	// Parsing client request
	std::string tmp(buffer, bytes);
	parsing_request_.fillBuffer(tmp);

	if (Signal::signal == 1)
		throw(SIGINT);

	if (parsing_request_.getStep() != FINISH)
		return 0;

	Request request = parsing_request_.getRequest();
	request.setCode(parsing_request_.getCode());

	std::string response_str;

	Location	location = getGoodLocation(server_.getLocations(), request);
	std::string path = location.buildPath(request);

	if (isCGI(request.getUri().getTarget()))
	{
		handleCGI(request, response_str, path, location);
		std::cout << "Connexion return \n";
		return (0);
	}
	handleHTTP(request, response_str, path, location);
	return (sendMsg(response_str));
}

int
Connection::sendMsg(std::string const& msg)
{
	send(fd_, msg.c_str(), msg.size(), 0);

	if (Signal::signal == 1)
		throw(SIGINT);

	return (1);
}

int
Connection::handleEvent(EpollManager& manager, uint32_t events)
{
	// std::cout << "Connexion handle event \n";
	// Adding the size_body_max inside the parsing_request;
	parsing_request_.setMaxBodyLength(server_.getMaxClientRequestBody());

	(void) manager;
	if ((events & (EPOLLERR | EPOLLRDHUP)) != 0)
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

Connection::Connection(int fd, Listener& server, std::string addr,
					   EpollManager& manager) :
	ASocket(fd), server_(server), manager_(manager), addr_client_(addr)
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
