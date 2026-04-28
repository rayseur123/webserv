#include "Body.hpp"
#include <string>
#include "utils.hpp"
#include <cstdlib>

std::string body_buff;

int    Body::chunkedBody(std::string &container)
{
    char *end = NULL;

    while (true)
    {
        size_t  pos;
        

        if (status_ == GETTING_LENGTH)
        {
            pos = container.find("\r\n");
            if (pos == std::string::npos)
                return 0;
            length_ = std::strtoul(container.substr(0,pos).c_str(),&end, 16);

            if (length_ == 0)
                return 1;

            container.erase(0,pos + 2);
            status_++;
        }
        if (status_ == READING)
        {
            if (container.size() < (size_t)length_ + 2)
                return 0;
            
            content_ += container.substr(0, length_);
            container.erase(0 , length_ + 2);;
            status_--;
        }
    }
}


int    Body::lengthBody(std::string &line)
{
    std::string tmp;

    if (writed_ < length_)
    {
        tmp = line.substr(0, length_ - writed_);
        content_.append(tmp);
        writed_ += tmp.length();
        line.erase(0, tmp.length());
    }

    if (writed_ >= length_)
        return 1;

    return 0;
}

void Body::setLength(int nb)
{
    length_ = nb;
}

void Body::setContent(std::string const& content)
{
    content_ = content;
}

void Body::setWrited(int writed)
{
    writed_ = writed;
}


int Body::getLength() const
{
    return length_;
}

std::string const& Body::getContent() const
{
    return content_;
}

int Body::getWrited() const
{
    return writed_;
}

Body::Body():
status_(0), writed_(0), length_(0)
{}

Body::Body(std::string content)
{
    content_ = content;
    writed_ = 0;
    length_ = 0;
    status_ = 0;
}

Body const& Body::operator=(Body const& to_copy)
{
    if (this != &to_copy)
        content_ = to_copy.content_;
    return *this;
}

Body::Body(Body const& to_copy)
{
    *this = to_copy;
}

Body::~Body()
{}

std::ostream& operator<<(std::ostream& os, Body const& m)
{
    os << m.getContent();
    return os;
}