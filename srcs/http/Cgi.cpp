#include "http/Cgi.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <utility>
#include "epoll/EpollManager.hpp"
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

bool
Cgi::isCgiProgram(std::string const& s)
{
	size_t pos = 0;

	pos = s.find(".py");
	if (pos != std::string::npos)
	{
		std::cout << "python inside" << std::endl;
		extension_ = "py";
	};

	pos = s.find(".php");
	if (pos != std::string::npos)
	{
		extension_ = "php";
	}

	std::cout << "extension:" << extension_;
	return (extension_.length() > 0);
}

void
Cgi::parseUri(Request const& r, std::string& path)
{

	size_t		pos = 0;
	std::string uri = path;
	std::string buff;

	if (*uri.begin() == '/')
		uri.erase(0, 1);

	pos = uri.find('/');
	if (pos == std::string::npos)
	{
		isCgiProgram(uri);
		path = uri;
		env_.push_back("SCRIPT_NAME=/" + uri);
		env_.push_back("PATH_INFO=");
		return;
	}
	while (pos != std::string::npos)
	{
		if (isCgiProgram(uri.substr(0, pos)))
		{
			buff += uri.substr(0, pos);
			uri.erase(0, pos);
			break;
		}
		buff += uri.substr(0, pos + 1);
		uri.erase(0, pos + 1);
		pos = uri.find('/');
	}

	if (pos == std::string::npos)
	{
		isCgiProgram(uri);
		path = buff + uri;
		env_.push_back("SCRIPT_NAME=/" + (buff + uri));
		env_.push_back("PATH_INFO=");
	}
	else
	{
		path = buff;
		env_.push_back("SCRIPT_NAME=/" + buff);
		env_.push_back("PATH_INFO=" + uri);
	}
	(void) r;
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
	env_.push_back("REQUEST_METHOD=" + r.getMethod().toString());

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

	// SCRIPT_NAME && PATH_INFO
	parseUri(r, path);

	// HEADER-META-VARIABLES
	addingRequestHeaderEnv(r);
}

// Faudra modifier le path info et le scriptname avec le path
void
Cgi::startProgram(Request const& r, Connection& c, std::string const& path,
				  Location const& location)
{
	(void) location;
	(void) path;

	int	  fds[2] = { 0 };
	pid_t status = 0;

	socketpair(AF_UNIX, SOCK_STREAM, 0, fds);

	// fds[0] part dans epoll
	// fds[1] part dans le execve

	write(fds[0], r.getBody().getContent().c_str(),
		  r.getBody().getContent().length());

	status = fork();

	if (status == -1)
		throw("une erreur");

	if (status == 0)
	{

		dup2(fds[1], 1);
		dup2(fds[1], 0);

		close(fds[0]);
		close(fds[1]);

		std::vector<std::string> argv;
		std::string				 exec;

		if (extension_ == "py")
			exec = location.getCgiPathMap().at("py");
		else if (extension_ == "php")
			exec = (location.getCgiPathMap().at("php"));

		std::cout << "extension :" << extension_ << std::endl;
		std::cout << exec << std::endl;

		argv.push_back(exec);
		argv.push_back(path);

		if (execve(exec.c_str(), convertToExecve(argv).data(),
				   convertToExecve(env_).data()) == -1)
		{
			std::cout << "serveur error iciciii cgi can't start" << std::endl;
		}
	}
	else
	{
		epoll_event ev = {};

		ev.events = EVENTS_CONNECTION;
		ev.data.fd = fds[0];

		close(fds[1]);

		SocketCgi* cgi_socket = new SocketCgi(c, fds[0], status);

		if (epoll_ctl(c.getManager().getEpollFd(), EPOLL_CTL_ADD, fds[0],
					  &ev) == -1)
		{
			std::cout << "serveur error iciciii ERROR 500 Internal serveur "
						 "error iciciii"
					  << std::endl;
		}
		c.getManager().addCgi(std::make_pair(fds[0], cgi_socket));
	}
}

Cgi::Cgi(Cgi const& c) : env_(c.env_)
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
