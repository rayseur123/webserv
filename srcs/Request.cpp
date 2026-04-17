#include "Request.hpp"
#include <sstream>

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

void Request::setHeader(Header const& type)
{
    type_ = type;
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
    return type_;
}

Body const& Request::getBody() const
{
    return body_;
}

Request::Request(){}

Request::Request(std::string request)
{
    std::stringstream file;
    char coucou[10000];

    file << request;
    file.getline(&coucou[0], 10000, '\n');
    std::cout << coucou << std::endl;
    
}

Request::Request(Request const& to_copy)
{
    *this = to_copy;
}

Request const& Request::operator=(Request const& to_copy)
{
    if (this != &to_copy)
    {
        method_  = to_copy.method_;
        uri_     = to_copy.uri_;
        version_ = to_copy.version_;
        type_    = to_copy.type_;
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
    os << "Header: "  << r.getHeader()    << std::endl;
    os << "Body: "    << r.getBody()    << std::endl;

    return os;
}