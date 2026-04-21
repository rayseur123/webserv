#include "Method.hpp"

void Method::setMethod(int const type)
{
    type_ = type;
}

int Method::getMethod() const
{
    return type_;
}

Method const& Method::operator=(Method const& to_copy)
{
    if (this != &to_copy)
        type_ = to_copy.type_;
    return *this;
}

bool    Method::operator==(Method const& m) const
{
    if (type_ == m.type_)
        return 1;
    return 0;
}

bool    Method::operator!=(Method const& m) const
{
    if (type_ != m.type_)
        return 1;
    return 0;
}

Method::Method():
    type_(0)
{}

Method::Method(std::string method)
{
    if (method == "GET")
        type_ = 1;
    else if (method == "POST")
        type_ = 2;
    else if (method == "DELETE")
        type_ = 4;
    else
        throw(std::logic_error("[Error] Method(Std::string) Invalid Verb"));
}

Method::Method(Method const& to_copy)
{
    *this = to_copy;
}    


Method::~Method()
{}

std::ostream & operator<<(std::ostream& os, Method const& m)
{
    switch (m.getMethod())
    {
    case 1:
        os << "GET";
        break;
    
    case 2:
        os << "POST";
        break;
    
    case 4:
        os << "DELETE";
        break;
    }
    return os;
}