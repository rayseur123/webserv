#include "Type.hpp"

void Type::setContentLength(int const length)
{
    content_length_ = length;
}

void Type::setContentType(std::string const& type)
{
    content_type_ = type;
}

int Type::getContentLength() const
{
    return content_length_;
}

std::string const& Type::getContentType() const
{
    return content_type_;
}

Type const& Type::operator=(Type const& to_copy)
{
    if (this != &to_copy)
    {
        content_length_ = to_copy.content_length_;
        content_type_   = to_copy.content_type_;
    }
    return *this;
}

Type::Type(): 
    content_length_(-1)
{}

Type::Type(std::string type)
{
    (void)type;
    //parsing;
}

Type::Type(Type const& to_copy)
{
    *this = to_copy;
}

Type::~Type()
{}

std::ostream& operator<<(std::ostream& os, Type const& m)
{
    os << "Content-Length: " << m.getContentLength() << "\n";
    os << "Content-Type: "   << m.getContentType();
    return os;
}