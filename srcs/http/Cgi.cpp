#include "http/Cgi.hpp"
#include <cstdio>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <utility>
#include "epoll/EpollManager.hpp"
#include "http/Code.hpp"
#include "http/httpStatus.hpp"
#include "socket/Connection.hpp"
#include "socket/SocketCgi.hpp"
#include "sys/socket.h"
#include "utils/utils.hpp"

namespace
{

	std::string
	prepareHeaderToEnv(std::string first, std::string const& second)
	{

		std::string			  final;
		std::string::iterator it;

		toUpperString(first);

		for (it = first.begin(); it != first.end(); it++)
		{
			if (*it == '-')
				*it = '_';
		}

		final = "HTTP_" + first + '=' + second;
		return final;
	}

	bool
	headerIsAllowedInEnv(std::string const& name)
	{
		return (name != "content-length" && name != "content-type" &&
				name != "authorization");
	}

	std::vector<char*>
	convertToExecve(std::vector<std::string> vec)
	{
		std::vector<char*>						 tmp;
		std::vector<std::string>::const_iterator it;

		for (it = vec.begin(); it != vec.end(); it++)
		{
			tmp.push_back(const_cast<char*>(it->c_str()));
		}
		tmp.push_back(NULL);
		return tmp;
	}

} // namespace

void
Cgi::defineExtension(std::string const& path)
{
	std::string check;
	size_t		pos = 0;

	pos = path.find(".py");
	if (pos != std::string::npos)
	{
		check = path.substr(pos);
		if (check.length() == 3)
			extension_ = "py";
	}
	pos = path.find(".php");
	if (pos != std::string::npos)
	{
		check = path.substr(pos);
		if (check.length() == 4)
			extension_ = "php";
	}
}

void
Cgi::addingRequestHeaderEnv(Request const& r)
{
	std::map<std::string, std::string>::const_iterator it;
	std::map<std::string, std::string> shortcut = r.getHeader().getHeaders();

	for (it = shortcut.begin(); it != shortcut.end(); it++)
	{
		if (headerIsAllowedInEnv(it->first))
			env_.push_back(prepareHeaderToEnv(it->first, it->second));
	}
}

void
Cgi::buildEnv(Request const& r, Listener const& s,
			  std::string const& addr_client, std::string& path)
{
	// REQUEST_METHOD
	if (r.getMethod().toString() == "DELETE")
		throw(Code(HTTP_METHOD_NOT_ALLOWED));
	env_.push_back("REQUEST_METHOD=" + r.getMethod().toString());

	// SCRIPT_NAME
	env_.push_back("SCRIPT_NAME=" + path);

	// SERVER_SOFTWARE
	env_.push_back("SERVER_SOFTWARE=Webserv/1.0");

	// REMOTE_ADDR
	env_.push_back("REMOTE_ADDR=" + addr_client);

	// GATEWAY_INTERFACE
	env_.push_back("GATEWAY_INTERFACE=CGI/1.1");

	// SERVER_NAME
	env_.push_back("SERVER_NAME=" + s.getAddress());

	// SERVER_PORT
	env_.push_back("SERVER_PORT=" + s.getPort());

	// SERVER_PROTOCOL
	env_.push_back("SERVER_PROTOCOL=" + r.getVersion().toString());

	// QUERY_STRING
	env_.push_back("QUERY_STRING=" + r.getUri().getQuery());

	// CONTENT_LENGTH && CONTENT_TYPE
	env_.push_back("CONTENT_LENGTH=" + r.getHeader().get("content-length"));
	if (r.getHeader().get("content-type").length() == 0)
		env_.push_back("CONTENT_TYPE=application/octet-stream");
	else
		env_.push_back("CONTENT_TYPE=" + r.getHeader().get("content-type"));

	// HEADER-META-VARIABLES
	addingRequestHeaderEnv(r);
}

void
Cgi::childProcess(int										fds[2],
				  std::map<std::string, std::string> const& path_map,
				  std::string const&						path)
{
	if (dup2(fds[1], 1) == -1)
		throw(ForkException());
	if (dup2(fds[1], 0) == -1)
		throw(ForkException());

	if (close(fds[0]) == -1)
		throw(ForkException());
	if (close(fds[1]) == -1)
		throw(ForkException());

	std::vector<std::string> argv;
	std::string				 exec;

	if (extension_ == "py")
		exec = path_map.at("py");
	else if (extension_ == "php")
		exec = (path_map.at("php"));

	argv.push_back(exec);
	argv.push_back(path);

	if (execve(exec.c_str(), convertToExecve(argv).data(),
			   convertToExecve(env_).data()) == -1)
	{
		perror("execve");
		throw(ForkException());
	}
}

void
parentProcess(int fds[2], pid_t status, Connection& c)
{
	epoll_event ev = {};

	ev.events = EVENTS_CONNECTION;
	ev.data.fd = fds[0];

	if (close(fds[1]) == -1)
	{
		perror("close_cgi");
		throw(Code(HTTP_INTERNAL_SERVER_ERROR));
	}

	SocketCgi* cgi_socket = new SocketCgi(c, fds[0], status);

	if (epoll_ctl(c.getManager().getEpollFd(), EPOLL_CTL_ADD, fds[0], &ev) ==
		-1)
	{
		perror("epoll_adding_cgi");
		throw Code(HTTP_INTERNAL_SERVER_ERROR);
	}
	c.getManager().addCgi(std::make_pair(fds[0], cgi_socket));
}

void
Cgi::startProgram(Request const& r, Connection& c, std::string const& path,
				  Location const& location)
{
	int	  fds[2] = {};
	pid_t status = 0;

	// fds[0] part dans epoll
	// fds[1] part dans le execve
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1)
	{
		perror("socketpair");
		throw(Code(HTTP_INTERNAL_SERVER_ERROR));
	}

	if (write(fds[0], r.getBody().getContent().c_str(),
			  r.getBody().getContent().length()) == -1)
	{
		perror("write_cgi");
		throw(Code(HTTP_INTERNAL_SERVER_ERROR));
	}

	if (access(path.c_str(), F_OK) != 0)
	{
		perror("access_cgi");
		throw(Code(HTTP_NOT_FOUND));
	}

	defineExtension(path);

	status = fork();

	if (status == -1)
	{
		perror("fork_cgi");
		throw(Code(HTTP_INTERNAL_SERVER_ERROR));
	}

	if (status == 0)
		childProcess(fds, location.getCgiPathMap(), path);
	else
		parentProcess(fds, status, c);
}

Cgi::Cgi(Cgi const& c) : env_(c.env_), extension_(c.extension_)
{}

Cgi&
Cgi::operator=(Cgi const& c)
{
	if (this != &c)
		env_ = c.env_;
	return *this;
}

Cgi::Cgi() {};

Cgi::~Cgi() {};

char const*
ForkException::what() const throw()
{
	return "error inside fork";
};
