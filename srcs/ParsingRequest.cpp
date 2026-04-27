#include "ParsingRequest.hpp"
#include <sstream>
#include "Request.hpp"
#include <vector>
#include "Error.hpp"

#include "utils.hpp"

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
    if (!keyIsValid(param))
        throw Error::ErrorException(400);
    if (param == "host")
        return 1;
    if (param == "content-length")
        return 1;
    if (param == "content-type")
        return 1;
    if (param == "transfer-encoding")
        return 1;
    return 0;
}

// Request LINE

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

// Header LINE

void ParsingRequest::headerLine(std::string line)
{
    std::vector<std::string> param;


    param = splitLineByDel(line, ':');
    if (param.size() < 2)
        throw Error::ErrorException(400);
    
    toLowerString(param[0]);
    if (headerIsAccepted(param[0]))
    {
        param[1].erase(0, 1);
        request_.addingInsideHeader(param);
    }
}


// Body LINE




void ParsingRequest::fillBuffer(std::string tmp)
{
    size_t pos;
    std::string line;

    buffer_.append(tmp);
    while (step_ != FINISH && step_ != BODY_CHUNCK)
    {
        pos = buffer_.find("\r\n");
        
        if (pos == std::string::npos)
            return;
        
        line = buffer_.substr(0, pos + 2);
        
        if (step_ == REQUEST)
        {
            line.erase(pos, pos + 2);
            requestLine(line);
            buffer_.erase(0, pos + 2);
            step_++;
        }
        else if (step_ == HEADER)
        {
            if (line == "\r\n")
                step_++;
            else
            {
                line.erase(pos, pos + 2);
                headerLine(line);
                buffer_.erase(0, pos + 2);
            }
        }
        else if (step_ == BODY)
        {
            if (request_.bodyIsLength())
                step_ += request_.addingBodyLength(buffer_);
            else if (request_.bodyIsChunked())
            {
                step_ = BODY_CHUNCK;
                break;
            }
            else 
            {
                step_++;
                return;
            }
        }
    }
    if (step_ == BODY_CHUNCK)
    {
        step_ += request_.addingBodyChunked(buffer_);
    }
}



































int ParsingRequest::getStep() const
{
    return step_;
}

Request& ParsingRequest::getRequest()
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
