#include <csignal>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "epoll/EpollManager.hpp"
#include "epoll/signal.hpp"
#include "http/Cgi.hpp"
#include "http/Code.hpp"
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
isCGI(std::string path, Location const& l)
{
	size_t		pos = 0;
	std::string extension;

	std::cout << path << std::endl;

	pos = path.rfind(".");
	if (pos != std::string::npos)
	{
		path.erase(0, pos + 1);
		extension = path;
		return l.hasCgiPathMap(extension);
	}
	return false;
}

int
Connection::handleConnectionRequest()
{
	size_t bytes = 0;
	char   buffer[10000] = {};

	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	if (bytes == 0 && isCgi_ == 1)
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

	if (access(path.c_str(), F_OK) != 0)
		request.setCode(HTTP_NOT_FOUND);

	if (request.getCode() != 0)
	{
		response_str = buildErrorResponse(request.getCode(), server_,
										  request.getVersion().toString());
		return (sendMsg(response_str));
	}

	if (isCGI(path, location))
	{
		std::cout << "Cgi accepted \n" << std::endl;
		isCgi_ = 1;
		return (handleCGI(request, response_str, path, location));
	}
	std::cout << "HTTP accepted \n" << std::endl;
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
	ASocket(fd), server_(server), manager_(manager), addr_client_(addr),
	isCgi_(0)
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
