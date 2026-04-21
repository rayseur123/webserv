#include "Body.hpp"

void Body::setContent(std::string const& content)
{
    content_ = content;
}

std::string const& Body::getContent() const
{
    return content_;
}


Body::Body(){}

Body::Body(std::string content)
{
    content_ = content;
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