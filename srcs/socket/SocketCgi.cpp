#include "http/httpStatus.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>

int SocketCgi::id_cookie = 0;

SocketCgi::SocketCgi(Connection& connection, int fd, pid_t pid) :
	ASocket(fd), connection_(connection), connection_fd_(connection.getFd()),
	pid_child_(pid)
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
defineIdSession()
{
	std::string		  cookie;
	std::stringstream ss;

	ss << SocketCgi::id_cookie++;

	cookie = "Set-Cookie:id=";
	cookie += ss.str();
	cookie += "; Path=/\r\n";

	return cookie;
}

std::string
SocketCgi::parsingResponseCgi(std::string& response)
{
	int status = fixSeparators(response);
	std::map<std::vector<int>, std::string> const& map =
		connection_.getServer().getErrorPage();
	std::map<std::vector<int>, std::string>::const_iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		if (std::find(it->first.begin(), it->first.end(), status) !=
			it->first.end())
			return (buildErrorResponse(status, connection_.getServer(),
									   "HTTP/1.0"));
	}
	return ("HTTP/1.0 " + getStatusMessage(status) + "\r\n" +
			defineIdSession() + response);
}

int
SocketCgi::handleEventCgi(EpollManager& manager)
{
	ssize_t bytes = 0;
	char	buffer[10000] = {};
	bytes = recv(fd_, buffer, sizeof(buffer), 0);
	response_ += buffer;
	if (bytes == -1)
	{
		connection_.sendMsg(buildErrorResponse(
			HTTP_INTERNAL_SERVER_ERROR, connection_.getServer(), "HTTP/1.0"));
		return (1);
	}
	if (bytes <= 0)
	{
		std::string response = parsingResponseCgi(response_);
		if (bytes == -1)
			connection_.sendMsg(buildErrorResponse(HTTP_INTERNAL_SERVER_ERROR,
												   connection_.getServer(),
												   "HTTP/1.0"));
		else
			connection_.sendMsg(response);
		manager.eraseFdSocketMap(connection_.getFd());
		return (1);
	}
	return (0);
}

int
SocketCgi::handleEvent(EpollManager& manager, uint32_t events)
{
	if (!manager.checkIfSocketOpen(connection_fd_))
	{
		kill(pid_child_, SIGTERM);
		return (1);
	}
	if ((events & (EPOLLERR)) != 0)
		return (1);
	if ((events & (EPOLLIN | EPOLLPRI)) != 0)
		return (handleEventCgi(manager));
	if ((events & (EPOLLHUP | EPOLLRDHUP)) != 0)
		return (1);
	return (0);
}

SocketCgi::~SocketCgi()
{}
