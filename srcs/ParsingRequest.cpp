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

void ParsingRequest::fill_request()
{
    // std::stringstream ss(buffer_);
    // std::vector<std::string> request_line;

    // request_line = splitLineByDel(getlineCRLF(ss), ' ');

    // if (request_line.size() != 3)
    //     throw Error::ErrorException(400);

    // Method const m(request_line[0]);
    // Uri const u(request_line[1]);
    // Version const v(request_line[2]);

    // request_.setMethod(m);
    // request_.setUri(u);
    // request_.setVersion(v);

    // Get the Headers param
    // std::string line;
    // std::vector<std::string> param;

    // while (std::getline(ss, line, '\n'))
    // {
    //     if (line == "\r")
    //         break;

    //     param = splitLineByDel(line, ':');
        
    //     if (headerIsAccepted(param[0]))
    //     {
    //         param[1].erase(0, 1);
    //         request_.getHeader().set(param[0], param[1]);
    //     }
    // }

    // Get the content of the body
    // if (header_.has("Content-Length"))
    // {
    //     std::string body_buff;

    //     size_t pos = ss.tellg();
    //     body_buff = request.substr(pos, header_.getContentLength());
        
    //     Body b(body_buff);
    //     body_ = b;
    // }
}

void ParsingRequest::requestLine()
{
    std::vector<std::string> request_line = splitLineByDel(buffer_, ' ');

    if (request_line.size() != 3)
        throw Error::ErrorException(400);

    Method const m(request_line[0]);
    Uri const u(request_line[1]);
    Version const v(request_line[2]);

    request_.setMethod(m);
    request_.setUri(u);
    request_.setVersion(v);
}

void ParsingRequest::headerLine()
{
    std::vector<std::string> param;


    param = splitLineByDel(buffer_, ':');
    
    if (headerIsAccepted(param[0]))
    {
        param[1].erase(0, 1);
        request_.getHeader().set(param[0], param[1]);
    }
}

void ParsingRequest::bodyLine()
{

}

void ParsingRequest::fillBuffer(std::string tmp)
{
    size_t pos;
    
    buffer_.append(tmp);
    while (true)
    {
        //Maybe problem than buffer contain all but not \r\n 
        pos = buffer_.find("\r\n");
        if (pos == std::string::npos)
            return;

        if (step_ == REQUEST)
        {
            requestLine();
            buffer_.erase(0, pos + 2);
            step_++;
        }
        else if (step_ == HEADER)
        {
            if (buffer_ == "\r\n" && request_.isValidForBody())
                step_++;
            else
            {
                headerLine();
                buffer_.erase(0, pos + 2);
            }
        }
        else if (step_ == BODY)
        {
            bodyLine();
            buffer_.erase(0, pos + 2);
            step_++;
        }
        std::cout << buffer_;
    }
}

ParsingRequest const& ParsingRequest::operator=(ParsingRequest const& to_copy)
{
    if (this == &to_copy)
        return *this;
    
    buffer_ = to_copy.buffer_;
    request_ = to_copy.request_;
    step_ = to_copy.step_;

    return;
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
