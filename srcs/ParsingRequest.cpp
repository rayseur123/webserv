#include "ParsingRequest.hpp"
#include <climits>
#include <cstddef>
#include "Request.hpp"
#include <vector>
#include "Error.hpp"
#include "utils.hpp"

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
void ParsingRequest::defineBodyType()
{
    Header tmp = request_.getHeader();
    if (tmp.has("content-type") && tmp.has("content-type"))
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

    line.erase(pos, pos + 2);

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

// Header LINE

void ParsingRequest::headerLine(std::string& line, size_t pos)
{
    std::vector<std::string> param;

    line.erase(pos, pos + 2);

    param = splitLineByDel(line, ':');
    if (param.size() < 2)
        throw Error::ErrorException(400);
    
    toLowerString(param[0]);\

    if (headerIsAccepted(param[0]))
    {
        param[1].erase(0, 1);
        request_.addingInsideHeader(param);
    }

    buffer_.erase(0, pos + 2);
}


// Body LINE

void ParsingRequest::bodyLine(std::string& line, size_t pos)
{
    (void)line;
    (void)pos;
}

// void ParsingRequest::bodyChunked(std::string& buffer)
// {

// }

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
            requestLine(line, pos);
        else if (step_ == HEADER)
        {
            if (line == "\r\n")
            {
                step_++;
                defineBodyType();
                if (body_type == NO_BODY || body_type == CHUNK_BODY)
                    return;
            }
            else
                headerLine(line, pos);
        }
        else if (step_ == BODY)
        {
            std::cout << "BODY_LINE" << std::endl;
            step_ += request_.addingBodyLength(line);
        }
    }
    if (step_ == CHUNK_BODY)
    {
        std::cout << "PAS DE BODY" << std::endl;
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
step_(0), body_type(0)
{}

ParsingRequest::~ParsingRequest()
{}
