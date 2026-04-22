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

void Request::setHeader(Header const& type)
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

std::string getlineCRLF(std::stringstream &ss)
{
    std::string line;

    std::getline(ss, line, '\n');

    if (*(line.end() - 1) == '\r')
        line.erase(line.end() -1);
    return line;
}


std::vector<std::string> splitLineByDel(std::string line, char del)
{
    std::vector<std::string> tmp;
    std::stringstream ss(line);
    std::string buffer;
    
    while (std::getline(ss, buffer, del))
        tmp.push_back(buffer);
    return (tmp);
}

bool headerIsAccepted(std::string param)
{
    if (param == "Host")
        return 1;
    if (param == "Content-Length")
        return 1;
    if (param == "Content-Type")
        return 1;
    return 0;
}

Request::Request(std::string &request)
{
    // Get the first line of the http request
    std::stringstream ss(request);
    std::vector<std::string> request_line;

    request_line = splitLineByDel(getlineCRLF(ss), ' ');

    if (request_line.size() != 3)
        throw Error::ErrorException(400);

    Method const m(request_line[0]);
    Uri const u(request_line[1]);
    Version const v(request_line[2]);

    method_ = m;
    uri_ = u;
    version_ = v;

    // Get the Headers param
    std::string line;
    std::vector<std::string> param;

    while (std::getline(ss, line, '\n'))
    {
        if (line == "\r")
            break;

        param = splitLineByDel(line, ':');
        
        if (headerIsAccepted(param[0]))
        {
            param[1].erase(0, 1);
            header_.set(param[0], param[1]);
        }
    }

    // Get the content of the body
    if (header_.has("Content-Length"))
    {
        std::string body_buff;

        size_t pos = ss.tellg();
        body_buff = request.substr(pos, header_.getContentLength());
        
        Body b(body_buff);
        body_ = b;
    }
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