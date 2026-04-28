#include "http/parsing/Request.hpp"
#include "http/parsing/Body.hpp"
#include "http/parsing/Header.hpp"
#include <vector>
#include <string>

void Request::setMethod(Method const& method)
{
    method_ = method;
}

void Request::setUri(Uri const& uri)
{
    uri_ = uri;
}

void Request::setVersion(Version const& version)
{
    version_ = version;
}

void Request::setHeader(Header& type)
{
    header_ = type;
}

void Request::setBody(Body const& body)
{
    body_ = body;
}

Method const& Request::getMethod() const
{
    return method_;
}

Uri const& Request::getUri() const
{
    return uri_;
}

Version const& Request::getVersion() const
{
    return version_;
}

Header Request::getHeader()
{
    return header_;
}

Body const& Request::getBody() const
{
    return body_;
}

Request::Request(){}


void Request::addingInsideHeader(std::vector<std::string> &param)
{
    header_.set(param[0], param[1]);
}   

bool Request::bodyIsLength() const
{
    if (header_.has("content-length") && header_.has("content-type"))
        return 1;
    return 0;
}

bool Request::bodyIsChunked() const
{
    if (header_.has("transfer-encoding"))
        return 1;
    return 0; 
}

int    Request::addingBodyLength(std::string &line)
{

    body_.setLength(header_.getContentLength());

    if (body_.lengthBody(line))
        return 1;
    return 0;
}

int    Request::addingBodyChunked(std::string &container)
{
    if (body_.chunkedBody(container))
        return 1;
    return 0;
}

Request::Request(Request const& to_copy)
{
    *this = to_copy;
}

Request& Request::operator=(Request const& to_copy)
{
    if (this != &to_copy)
    {
        method_  = to_copy.method_;
        uri_     = to_copy.uri_;
        version_ = to_copy.version_;
        header_  = to_copy.header_;
        body_    = to_copy.body_;
    }
    return *this;
}

Request::~Request()
{}

std::ostream& operator<<(std::ostream& os, Request& r)
{
    os << "Method: "  << r.getMethod()  << std::endl;
    os << "URI: "     << r.getUri()     << std::endl;
    os << "Version: " << r.getVersion() << std::endl;
    os << "Header: "  << std::endl <<  r.getHeader();
    os << "Body: "    << r.getBody()    << std::endl;

    return os;
}