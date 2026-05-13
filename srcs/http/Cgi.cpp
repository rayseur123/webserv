#include "http/Cgi.hpp"
#include <unistd.h>

// /usr/test/cgi.py/coucou/ca/va
// /
// /cgi.py/test
// /cgi.py
namespace
{

	bool
	isCgiProgram(std::string const& s)
	{
		size_t pos = 0;
		pos = s.find(".py");

		return (pos != std::string::npos);
	}

	void
	displayEnv(std::vector<std::string> list)
	{
		std::vector<std::string>::iterator it;

		for (it = list.begin(); it != list.end(); it++)
		{
			std::cout << *it << std::endl;
		}
	}
} // namespace

void
Cgi::parseUri(Request const& r)
{
	size_t		pos = 0;
	std::string uri = r.getUri().getTarget();

	if (*uri.begin() == '/')
		uri.erase(0, 1);

	pos = uri.find('/');
	while (pos != std::string::npos)
	{
		if (isCgiProgram(uri.substr(0, pos)))
		{
			env_.push_back("HTTP_SCRIPT_NAME=" + uri.substr(0, pos));
			break;
		}
		uri.erase(0, pos + 1);
		pos = uri.find('/');
	}
	uri.erase(0, pos);
	env_.push_back("HTTP_PATH_INFO=" + uri);
}

void
Cgi::buildEnv(Request const& r, Listener const& s,
			  std::string const& addr_client)
{
	// REQUEST_METHOD
	env_.push_back("HTTP_REQUEST_METHOD=" + r.getMethod().toString());

	// SERVER_SOFTWARE
	env_.push_back("HTTP_SERVER_SOFTWARE=");

	// REMOTE_ADDR
	env_.push_back("HTTP_REMOTE_ADDR=" + addr_client);

	// GATEWAY_INTERFACE
	env_.push_back("HTTP_GATEWAY_INTERFACE=CGI/1.1");

	// SERVER_NAME = C'est quoi le name du server
	env_.push_back("HTTP_SERVER_NAME=" + s.getAddress());

	// SERVER_PORT
	env_.push_back("HTTP_SERVER_PORT=" + s.getPort());

	// SERVER_PROTOCOL
	env_.push_back("HTTP_SERVER_PROTOCOL=" + r.getVersion().toString());

	// QUERY_STRING
	env_.push_back("HTTP_QUERRY_STRING=" + r.getUri().getQuery());

	// CONTENT_LENGTH && CONTENT_TYPE
	if (r.getMethod().toString() == "POST")
	{
		env_.push_back("CONTENT_LENGTH=" + r.getHeader().get("content-length"));
		env_.push_back("CONTENT_TYPE=" + r.getHeader().get("content-type"));
		// if (CONTENT_TYPE.length() == 0)
		//{
		// CONTENT_TYPE = content-type par default genre application/oxctstream
		//}
	}

	// SCRIPT_NAME && PATH_INFO
	parseUri(r);
	displayEnv(env_);
}

void
Cgi::startProgram() const
{}

Cgi::Cgi(Cgi const& c)
{
	env_ = c.env_;
}

Cgi&
Cgi::operator=(Cgi const& c)
{
	if (this == &c)
		env_ = c.env_;
	return *this;
}

Cgi::Cgi() {};

Cgi::~Cgi() {};
