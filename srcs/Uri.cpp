#include "Uri.hpp"

void Uri::setTarget(std::string const& target)
{
    if (isValid(target))
        target_ = target;
}

bool Uri::isValid(std::string const& target) const
{
    // Relative
    if(*target.begin() == '/')
    {
        return 1;
    }
}

std::string const& Uri::getTarget() const
{
    return target_;
}

Uri const& Uri::operator=(Uri const& to_copy)
{
    if (this != &to_copy)
    {
        target_ = to_copy.target_;
    }
    return *this;
}


Uri::Uri(){}

Uri::Uri(std::string uri):
    target_(uri)
{}

Uri::Uri(Uri const& to_copy)
{
    *this = to_copy;
}

Uri::~Uri()
{}

std::ostream& operator<<(std::ostream& os, Uri const& m)
{
    os << m.getTarget();
    return os;
}