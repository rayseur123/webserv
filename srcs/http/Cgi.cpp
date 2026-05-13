#include "http/Cgi.hpp"
#include <iostream>
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
} // namespace

void
Cgi::buildEnv(Request const& r, Listener const& s)
{
	std::string SCRIPT_NAME;
	std::string REQUEST_METHOD;
	std::string GATEWAY_INTERFACE;
	std::string REMOTE_ADDR;
	std::string SERVER_NAME;
	std::string QUERY_STRING;
	std::string CONTENT_LENGTH;
	std::string CONTENT_TYPE;
	std::string PATH_INFO;

	// REQUEST_METHOD
	std::string line = "HTTP_REQUEST_METHOD=" + r.getMethod().toString();
	env_.push_back(line);

	// GATEWAY_INTERFACE
	line = "HTTP_GATEWAY_INTERFACE=CGI/1.1";
	env_.push_back(line);

	// REMOTE_ADDR = ;

	// SERVER_NAME = C'est quoi le name du server

	// SERVER_PORT
	line = "HTTP_SERVER_PORT=" + s.getPort();
	env_.push_back(line);

	// SERVER_PROTOCOL
	line = "HTTP_SERVER_PROTOCOL=" + r.getVersion().toString();
	env_.push_back(line);

	// QUERY_STRING
	line = "HTTP_QUERRY_=" + r.getUri().getQuery();
	env_.push_back(line);

	// CONTENT_LENGTH && CONTENT_TYPE
	if (REQUEST_METHOD == "POST")
	{
		CONTENT_LENGTH = r.getHeader().get("content-length");

		CONTENT_TYPE = r.getHeader().get("content-type");
		// if (CONTENT_TYPE.length() == 0)
		//{
		// CONTENT_TYPE = content-type par default genre application/oxctstream
		//}
	}

	// SCRIPT_NAME && PATH_INFO
	parseUri(SCRIPT_NAME, PATH_INFO, r);
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
