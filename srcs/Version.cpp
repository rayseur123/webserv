#include "Version.hpp"
#include <sstream> 
#include <string>
#include <cstdlib>


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
    std::string proto;
    std::string vers;

    std::cout << version << std::endl;
    std::cout << version.length() << std::endl;
    std::getline(ss, proto, '/');
    if (proto != "HTTP")
        throw(std::logic_error("[Error] Version(Std::string) Protocol's Name"));
    protocol_ = proto;

    std::getline(ss, vers, '.');
    if (vers.length() != 1)
        throw(std::logic_error("[Error] Version(Std::string) Digit's length"));
    std::cout << vers << std::endl;
    first_nb_ = convertInNb(vers);

    ss >> vers;
    if (vers.length() != 1)
        throw(std::logic_error("[Error] Version(Std::string) Digit's length"));
    std::cout << vers << std::endl;
    sec_nb_ = convertInNb(vers);
}

int Version::convertInNb(std::string buffer)
{
    int nb;

    if (!isdigit(buffer.c_str()[0]))
        throw(std::logic_error("[Error] Version(std::string) Need Digit"));
    
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
