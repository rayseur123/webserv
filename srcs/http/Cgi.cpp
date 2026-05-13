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

} // namespace

void
parseUri(std::string& SCRIPT_NAME, std::string& PATH_INFO, Request const& r)
{
	size_t		pos = 0;
	std::string uri = r.getUri().getTarget();

	if (*uri.begin() == '/')
		uri.erase(0, 1);

	pos = uri.find('/');
	while (pos != std::string::npos)
	{
		if (SCRIPT_NAME.length() == 0 && isCgiProgram(uri.substr(0, pos)))
		{
			SCRIPT_NAME = uri.substr(0, pos);
			break;
		}
		uri.erase(0, pos + 1);
		pos = uri.find('/');
	}
	uri.erase(0, pos + 1);
	PATH_INFO = uri;
}

void
Cgi::buildEnv(Request const& r, Listener const& s)
{
	std::string SCRIPT_NAME;
	std::string REQUEST_METHOD;
	std::string GATEWAY_INTERFACE;
	std::string REMOTE_ADDR;
	std::string SERVER_NAME;
	std::string CONTENT_LENGTH;
	std::string CONTENT_TYPE;
	std::string PATH_INFO;

	// REQUEST_METHOD
	env_.push_back("HTTP_REQUEST_METHOD=" + r.getMethod().toString());

	// GATEWAY_INTERFACE
	env_.push_back("HTTP_GATEWAY_INTERFACE=CGI/1.1");

	// REMOTE_ADDR = ;

	// SERVER_NAME = C'est quoi le name du server

	// SERVER_PORT
	env_.push_back("HTTP_SERVER_PORT=" + s.getPort());

	// SERVER_PROTOCOL
	env_.push_back("HTTP_SERVER_PROTOCOL=" + r.getVersion().toString());

	// QUERY_STRING
	env_.push_back("HTTP_QUERRY_STRING=" + r.getUri().getQuery());

	// CONTENT_LENGTH && CONTENT_TYPE
	if (REQUEST_METHOD == "POST")
	{
		env_.push_back("CONTENT_LENGTH=" + r.getHeader().get("content-length"));
		env_.push_back("CONTENT_TYPE=" + r.getHeader().get("content-type"));
		// if (CONTENT_TYPE.length() == 0)
		//{
		// CONTENT_TYPE = content-type par default genre application/oxctstream
		//}
	}

	// SCRIPT_NAME && PATH_INFO
	// parseUri(r);
}

void
displayEnv(std::vector<std::string>)
{}

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
