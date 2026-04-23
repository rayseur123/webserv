#include "ParsingRequest.hpp"
#include <iostream>
#include <sstream>
#include "Request.hpp"
#include <vector>
#include "Error.hpp"

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

void ParsingRequest::requestLine(std::string line)
{
    std::vector<std::string> request_line = splitLineByDel(line, ' ');

    if (request_line.size() != 3)
        throw Error::ErrorException(400);

    Method const m(request_line[0]);
    Uri const u(request_line[1]);
    Version const v(request_line[2]);

    request_.setMethod(m);
    request_.setUri(u);
    request_.setVersion(v);
}

void ParsingRequest::headerLine(std::string line)
{
    std::vector<std::string> param;


    param = splitLineByDel(line, ':');
    
    if (headerIsAccepted(param[0]))
    {
        param[1].erase(0, 1);
        request_.addingInsideHeader(param);
    }
}

void ParsingRequest::bodyLine(std::string line)
{
    (void)line;
}

void ParsingRequest::fillBuffer(std::string tmp)
{
    size_t pos;
    std::string line;

    buffer_.append(tmp);
    while (step_ != FINISH)
    {
        pos = buffer_.find("\r\n");
        
        if (pos == std::string::npos)
            return;
        
        line = buffer_.substr(0, pos);
        
        if (step_ == REQUEST)
        {
            requestLine(line);
            buffer_.erase(0, pos + 2);
            step_++;
        }
        else if (step_ == HEADER)
        {
            if (buffer_ == "\r\n")
                step_++;
            else
            {
                headerLine(line);
                buffer_.erase(0, pos + 2);
            }
        }
        else if (step_ == BODY)
        {
            if (!request_.isValidForBody())
            {
                step_++;
                return;
            }
            bodyLine(line);
            buffer_.erase(0, pos + 2);
            step_++;
        }
    }
}

int ParsingRequest::getStep() const
{
    return step_;
}

Request const& ParsingRequest::getRequest() const
{
    return request_;
}

ParsingRequest const& ParsingRequest::operator=(ParsingRequest const& to_copy)
{
    if (this == &to_copy)
        return *this;
    
    buffer_ = to_copy.buffer_;
    request_ = to_copy.request_;
    step_ = to_copy.step_;

    return *this;
}

ParsingRequest::ParsingRequest(ParsingRequest const& to_copy)
{
    *this = to_copy;
}

ParsingRequest::ParsingRequest():
step_(0)
{}

ParsingRequest::~ParsingRequest()
{}
