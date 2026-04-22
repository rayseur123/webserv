#include "Version.hpp"
#include <sstream> 
#include <string>
#include <cstdlib>
#include "Error.hpp"


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
    first_nb_(1),
    sec_nb_(0)
{}

Version::Version(std::string version)
{
    std::stringstream ss(version);
    std::string tmp;

    if (version.length() != 8)
        throw Error::ErrorException(400);

    std::getline(ss, tmp, '/');
    if (tmp != "HTTP")
        throw Error::ErrorException(400);
    protocol_ = tmp;

    std::getline(ss, tmp, '.');
    if (tmp.length() != 1)
        throw Error::ErrorException(400);
    first_nb_ = convertInNb(tmp);

    ss >> tmp;
    if (tmp.length() != 1)
        throw Error::ErrorException(400);
    sec_nb_ = convertInNb(tmp);

    isValid();
}

void Version::isValid() const
{
    if (first_nb_ != 1 || sec_nb_ !=  0)
        throw Error::ErrorException(505);
}

int Version::convertInNb(std::string buffer)
{
    int nb;

    if (!isdigit(buffer.c_str()[0]))
        throw Error::ErrorException(400);
    
    nb = atoi(buffer.c_str());
    return (nb);
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
