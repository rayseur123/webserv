#include "socket/SocketCgi.hpp"

#include <csignal>
#include <cstddef>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

SocketCgi::SocketCgi(Connection& connection, int fd, pid_t pid) :
	connection_(connection), fd_(fd), pid_child_(pid)
{}

std::string
SocketCgi::parsingResponseCgi(std::string const& response)
{
	std::string status_line("HTTP/1.0 200 OK\r\n");
	return (status_line + response);
}

int
SocketCgi::handleEventCgi()
{
	size_t bytes = 0;
	char   buffer[10000] = {};
	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	response_ += buffer;
	if (bytes == 0)
	{
		std::string response = parsingResponseCgi(response_);
		connection_.sendMsg(response);
		return (1);
	}
	return (0);
}

int
SocketCgi::handleEvent(EpollManager& manager, uint32_t events)
{
	(void) manager;
	if ((events & (EPOLLERR | EPOLLRDHUP)) != 0)
		return (1);
	if ((events & (EPOLLIN | EPOLLPRI)) != 0)
		return (handleEventCgi());
	return (0);
}

SocketCgi::~SocketCgi()
{}
