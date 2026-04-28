#include "http/parsing/ParsingRequest.hpp"
#include <climits>
#include <cstddef>
#include "http/parsing/Request.hpp"
#include <sstream>
#include <string>
#include <vector>
#include "http/Error.hpp"
#include "utils/utils.hpp"
#include <cstring>

// Request LINE
void ParsingRequest::defineBodyType()
{
    Header tmp = request_.getHeader();
    if (tmp.has("content-type") && tmp.has("content-length"))
        body_type = LINE_BODY;
    else if (tmp.has("transfer-encoding"))
        body_type = CHUNK_BODY;
    else
    {
        body_type = NO_BODY;
        step_++;
    }
}

void ParsingRequest::requestLine(std::string& line, size_t pos)
{
    std::vector<std::string> request_line;

    line.erase(pos, 2);
    request_line = splitLineByDel(line, ' ');
    
    if (request_line.size() != 3)
        throw Error::ErrorException(400);

    Method const m(request_line[0]);
    Uri const u(request_line[1]);
    Version const v(request_line[2]);

    request_.setMethod(m);
    request_.setUri(u);
    request_.setVersion(v);

    buffer_.erase(0, pos + 2);
    step_++;
}

std::vector<std::string> splitHeader(std::string &line)
{
    std::stringstream ss;
    std::string tmp;
    std::vector<std::string> buffer;

    ss << line;

    std::getline(ss, tmp, ':');
    buffer.push_back(tmp);
    std::getline(ss, tmp, ':');
    buffer.push_back(tmp);
    
    return buffer;
}


void ParsingRequest::headerLine(std::string& line, size_t pos)
{
    std::vector<std::string> param;
    line.erase(pos, 2);

    param = splitHeader(line);
    toLowerString(param[0]);
    // if (keyIsValid(param[0]))
    // {
    //     throw Error::ErrorException(400);
    // }
    trimSpaceString(param[1]);

    request_.addingInsideHeader(param);
    buffer_.erase(0, pos + 2);

}

// Parsing Management

void ParsingRequest::fillBuffer(std::string& tmp)
{
    size_t pos;
    std::string line;

    buffer_.append(tmp);
    while (step_ != FINISH && body_type != CHUNK_BODY && body_type != NO_BODY)
    {
        pos = buffer_.find("\r\n");
        if (pos == std::string::npos)
            return;
        
        line = buffer_.substr(0, pos + 2);
        
        if (step_ == REQUEST)
        {
            requestLine(line, pos);
        }
        else if (step_ == HEADER)
        {
            if (line == "\r\n")
            {
                step_++;
                defineBodyType();
                buffer_.erase(0, 2);
                if (body_type == NO_BODY || body_type == CHUNK_BODY)
                    break;
            }
            else
                headerLine(line, pos);
        }
        if (step_ == BODY)
            step_ += request_.addingBodyLength(buffer_);
    }
    if (step_ == CHUNK_BODY)
        step_ += request_.addingBodyChunked(buffer_);
}



































int ParsingRequest::getStep() const
{
    return step_;
}

Request& ParsingRequest::getRequest()
{
    return request_;
}

ParsingRequest& ParsingRequest::operator=(ParsingRequest const& to_copy)
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
step_(0), body_type(0)
{}

ParsingRequest::~ParsingRequest()
{}
