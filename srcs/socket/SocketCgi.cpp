#include "socket/SocketCgi.hpp"
#include "http/httpStatus.hpp"
#include "utils/utils.hpp"

#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

SocketCgi::SocketCgi(Connection& connection, int fd, pid_t pid) :
	connection_(connection), fd_(fd), pid_child_(pid)
{}

int
checkStatusIsGood(std::string& response)
{
	size_t index = response.find("Status");
	if (index == std::string::npos)
		return (HTTP_OK);

	size_t end_of_line = response.find('\n', index);
	if (end_of_line == std::string::npos)
		return (HTTP_INTERNAL_SERVER_ERROR);

	std::string status_line = response.substr(index, end_of_line - index);

	size_t space_pos = status_line.find(' ');
	if (space_pos == std::string::npos)
		return (HTTP_INTERNAL_SERVER_ERROR);

	std::string code_str = status_line.substr(space_pos + 1, 3);
	int			status_code = std::atoi(code_str.c_str());

	if (status_code < 100 || status_code > 599)
		return (HTTP_INTERNAL_SERVER_ERROR);
	response.erase(index, status_line.size() + 1);
	return (status_code);
}

int
fixSeparators(std::string& response)
{
	size_t index = response.find("\n\n");
	if (index == std::string::npos)
		return (HTTP_INTERNAL_SERVER_ERROR);

	std::string body = response.substr(index + 2);
	response.resize(index);

	size_t i = 0;
	int	   status = checkStatusIsGood(response);
	while ((i = response.find('\n', i)) != std::string::npos)
	{
		response.replace(i, 1, "\r\n");
		i += 2;
	}
	response = response + "\r\n\r\n" + body;
	return (status);
}

std::string
SocketCgi::parsingResponseCgi(std::string& response)
{
	int status = fixSeparators(response);
	return ("HTTP/1.0 " + getStatusMessage(status) + "\r\n" + response);
}

int
SocketCgi::handleEventCgi()
{
	std::cout << "Je suis la" << std::endl;

	size_t bytes = 0;
	char   buffer[10000] = {};
	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	response_ += buffer;

	std::cout << response_ << std::endl;

	if (bytes == 0)
	{
		std::string response = parsingResponseCgi(response_);
		std::cout << "Response CGI \n" << response << std::endl;
		connection_.sendMsg(response);
		return (1);
	}
	return (0);
}

int
SocketCgi::handleEvent(EpollManager& manager, uint32_t events)
{
	std::cout << "HANDDDLELLLE EVEENNT" << std::endl;
	(void) manager;
	if ((events & (EPOLLERR | EPOLLRDHUP)) != 0)
		return (1);
	if ((events & (EPOLLIN | EPOLLPRI)) != 0)
		return (handleEventCgi());
	return (0);
}

SocketCgi::~SocketCgi()
{}
