#include "Body.hpp"
#include <string>
#include "utils.hpp"

std::string body_buff;

int    Body::chunkedBody(std::string &container)
{
    size_t pos;
    int size;

    
    pos = container.find("\r\n");
    if (pos == std::string::npos)
    {
        return 0;
        // Return an error inside the syntax isnide the body
    }

    size = stoi(container.substr(0, pos));
    
    if (size == 0)
        return 1;

    container.erase(0, pos + 2);

    content_ += container.substr(0, size);
    
    // Peut avoir un soucis ici si le container est plus petit que 2.
    container.erase(0 , size + 2);
    return 0;
}


int    Body::lengthBody(std::string &line)
{
    std::string tmp;

    std::cout << writed_ << std::endl;
    std::cout << length_ << std::endl;
    if (writed_ < length_)
    {
        // Check if it's okay to put a bigger number than potentialy the size of the string
        tmp = line.substr(0, length_ - writed_);
        content_.append(tmp);
        std::cout << "Content" << content_ << std::endl;
        writed_ += tmp.length();
        line.erase(0, tmp.length());
        return 0;
    }
    return 1;
}

void Body::setLength(int nb)
{
    length_ = nb;
}

void Body::setContent(std::string const& content)
{
    content_ = content;
}


int Body::getLength() const
{
    return length_;
}

std::string const& Body::getContent() const
{
    return content_;
}

Body::Body():
writed_(0), length_(0)
{}

Body::Body(std::string content)
{
    content_ = content;
    writed_ = 0;
    length_ = 0;
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