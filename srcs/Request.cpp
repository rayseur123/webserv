#include "Request.hpp"
#include <sstream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <string>
#include "Error.hpp"

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


Request::Request(std::string &request)
{
    (void)request;
    // // Get the content of the body
    // if (header_.has("Content-Length"))
    // {
    //     std::string body_buff;

    //     size_t pos = ss.tellg();
    //     body_buff = request.substr(pos, header_.getContentLength());
        
    //     Body b(body_buff);
    //     body_ = b;
    // }
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