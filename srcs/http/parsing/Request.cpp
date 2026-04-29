#include "http/parsing/Request.hpp"
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include "http/parsing/Body.hpp"
#include "http/parsing/Header.hpp"

void
Request::setMethod(Method const& method)
{
	method_ = method;
}

void
Request::setMethod(std::string const& method)
{
	method_ = Method(method);
}

void
Request::setUri(Uri const& uri)
{
	uri_ = uri;
}

void
Request::setUri(std::string const& target)
{
	uri_ = Uri(target);
}

void
Request::setVersion(Version const& version)
{
	version_ = version;
}

void
Request::setVersion(std::string const& version)
{
	version_ = Version(version);
}

void
Request::setHeaders(Headers& type)
{
	headers_ = type;
}

void
Request::setBody(Body const& body)
{
	body_ = body;
}

Method const&
Request::getMethod() const
{
	return method_;
}

Uri const&
Request::getUri() const
{
	return uri_;
}

Version const&
Request::getVersion() const
{
	return version_;
}

Headers
Request::getHeader()
{
	return headers_;
}

Body const&
Request::getBody() const
{
	return body_;
}

Request::Request()
{}

void
Request::addingInsideHeader(std::pair<std::string, std::string>& head)
{
	headers_.set(head.first, head.second);
}

bool
Request::bodyIsLength() const
{
	return (headers_.has("content-length") && headers_.has("content-type"));
}

bool
Request::bodyIsChunked() const
{
	return (headers_.has("transfer-encoding"));
}

int
Request::addingBodyLength(std::string& line)
{
	body_.setLength(headers_.getContentLength());

	return (body_.lengthBody(line));
}

int
Request::addingBodyChunked(std::string& container)
{
	return (body_.chunkedBody(container));
}

Request::Request(Request const& to_copy)
{
	*this = to_copy;
}

Request&
Request::operator=(Request const& to_copy)
{
	if (this != &to_copy)
	{
		method_ = to_copy.method_;
		uri_ = to_copy.uri_;
		version_ = to_copy.version_;
		headers_ = to_copy.headers_;
		body_ = to_copy.body_;
	}
	return *this;
}

Request::~Request()
{}

std::ostream&
operator<<(std::ostream& os, Request& r)
{
	os << "Method: " << r.getMethod() << '\n';
	os << "URI: " << r.getUri() << '\n';
	os << "Version: " << r.getVersion() << '\n';
	os << "Header: " << '\n' << r.getHeader();
	os << "Body: " << r.getBody() << '\n';

	return os;
}
