#include "http/parsing/Request.hpp"
#include <string>
#include <vector>
#include "http/parsing/Body.hpp"
#include "http/parsing/Header.hpp"

void
Request::setMethod(Method const& method)
{
	method_ = method;
}

void
Request::setUri(Uri const& uri)
{
	uri_ = uri;
}

void
Request::setVersion(Version const& version)
{
	version_ = version;
}

void
Request::setHeader(Header& type)
{
	header_ = type;
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

Header
Request::getHeader()
{
	return header_;
}

Body const&
Request::getBody() const
{
	return body_;
}

Request::Request()
{}

void
Request::addingInsideHeader(std::vector<std::string>& param)
{
	header_.set(param[0], param[1]);
}

bool
Request::bodyIsLength() const
{
	return (header_.has("content-length") && header_.has("content-type"));
}

bool
Request::bodyIsChunked() const
{
	return (header_.has("transfer-encoding"));
}

int
Request::addingBodyLength(std::string& line)
{
	body_.setLength(header_.getContentLength());

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
		header_ = to_copy.header_;
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
