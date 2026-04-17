#include "Header.hpp"
#include "algorithm"

void    Header::set(const std::string &key, const std::string &value)
{
    headers_[key] = value;
}

std::string& Header::get(const std::string &key)
{
    return headers_[key];
}

bool    Header::has(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it;

    it = headers_.find(key);
    if (it != headers_.end())
        return 1;
    return 0;
}

Header const& Header::operator=(Header const& to_copy)
{
    headers_ = to_copy.headers_;   
    return *this;
}

Header::Header() 
{}


Header::Header(Header const& to_copy)
{
    *this = to_copy;
}

Header::~Header()
{}

std::map<std::string, std::string> const& Header::getHeaders() const
{
    return headers_;
}

std::ostream& operator<<(std::ostream& os, Header const& m)
{
    std::map<std::string, std::string>::const_iterator it;

    for(it = m.getHeaders().begin(); it != m.getHeaders().end(); ++it)
    {
        os << "Type: " << it->first << std::endl;
        os << "Value: " << it->second << std::endl;
    }
    return os;
}