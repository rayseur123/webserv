#include "Request.hpp"
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

Header const& Request::getHeader() const
{
    return header_;
}

Body const& Request::getBody() const
{
    return body_;
}

Request::Request(){}


void Request::addingInsideHeader(std::vector<std::string> param)
{
    header_.set(param[0], param[1]);
}   

bool Request::isValidForBody() const
{
    if (header_.has("Content-Length") && header_.has("Content-Type"))
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

std::ostream& operator<<(std::ostream& os, Request const& r)
{
    os << "Method: "  << r.getMethod()  << std::endl;
    os << "URI: "     << r.getUri()     << std::endl;
    os << "Version: " << r.getVersion() << std::endl;
    os << "Header: "  << std::endl << r.getHeader()  << std::endl;
    os << "Body: "    << r.getBody()    << std::endl;

    return os;
}