#include "http/parsing/Request.hpp"
#include <string>
#include <utility>
#include "http/parsing/Body.hpp"
#include "http/parsing/Header.hpp"
#include "http/parsing/Uri.hpp"

void
Request::setCode(int code)
{
	code_ = code;
}

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
Request::setVersion(std::string& version)
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

int
Request::getCode() const
{
	return code_;
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
Request::getHeader() const
{
	return headers_;
}

Body const&
Request::getBody() const
{
	return body_;
}

Request::Request() : code_(0)
{}

void
Request::addingInsideHeader(std::pair<std::string, std::string>& head)
{
	headers_.set(head.first, head.second);
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

Request::Request(Request const& to_copy) :
	method_(to_copy.method_), uri_(to_copy.uri_), version_(to_copy.version_),
	headers_(to_copy.headers_), body_(to_copy.body_), code_(to_copy.code_)
{}

void
Request::resetRequest()
{
	method_ = Method();
	uri_ = Uri();
	version_ = Version();
	headers_ = Headers();
	body_ = Body();
}

Request&
Request::operator=(Request const& to_copy)
{
	if (this == &to_copy)
		return *this;
	method_ = to_copy.method_;
	uri_ = to_copy.uri_;
	version_ = to_copy.version_;
	headers_ = to_copy.headers_;
	body_ = to_copy.body_;
	return *this;
}

Request::~Request()
{}

std::ostream&
operator<<(std::ostream& os, Request const& r)
{
	os << "Method: " << r.getMethod() << '\n';
	os << "URI: " << r.getUri() << '\n';
	os << "Version: " << r.getVersion() << '\n';
	os << "Header: " << '\n' << r.getHeader();
	os << "Body: " << r.getBody() << '\n';

	return os;
}
