#include "Version.hpp"

void Version::setProtocol(std::string protocol)
{
    protocol_ = protocol;
}

void Version::setFirstNb(int nb)
{
    first_nb_ = nb;
}

void Version::setSecNb(int nb)
{
    sec_nb_ = nb;
}

std::string Version::getProtocol() const
{
    return protocol_;
}

int Version::getFirstNb() const
{
    return first_nb_;
}

int Version::getSecNb() const
{
    return sec_nb_;
}

Version::Version(Version const& to_copy)
{
    *this = to_copy;
}

Version::Version():
    protocol_("HTTP"),
    first_nb_(0),
    sec_nb_(9)
{}

Version::Version(std::string version)
{
    (void)version;
    //Parsing
}


Version const& Version::operator=(Version const& to_copy)
{
    if (this != &to_copy)
    {
        protocol_ = to_copy.protocol_;
        first_nb_ = to_copy.first_nb_;
        sec_nb_   = to_copy.sec_nb_;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, Version const& m)
{
    os << m.getProtocol() << "/" << m.getFirstNb() << "." << m.getSecNb();
    return os;
}


Version::~Version()
{}
