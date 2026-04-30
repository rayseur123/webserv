#include "http/parsing/Version.hpp"
#include <cstdlib>
#include "http/Error.hpp"
#include "utils/utils.hpp"

void
Version::setProtocol(std::string const& protocol)
{
	protocol_ = protocol;
}

void
Version::setFirstNb(int nb)
{
	first_nb_ = nb;
}

void
Version::setSecNb(int nb)
{
	sec_nb_ = nb;
}

std::string
Version::getProtocol() const
{
	return protocol_;
}

int
Version::getFirstNb() const
{
	return first_nb_;
}

int
Version::getSecNb() const
{
	return sec_nb_;
}

Version::Version(Version const& to_copy) :
	first_nb_(to_copy.first_nb_), sec_nb_(to_copy.sec_nb_)
{}

Version::Version() : protocol_("HTTP"), first_nb_(1), sec_nb_(0)
{}

Version::Version(std::string const& version)
{
	std::stringstream ss(version);
	std::string		  tmp;

	std::getline(ss, tmp, '/');
	if (tmp != "HTTP")
		throw Error::ErrorException(400);
	protocol_ = tmp;

	std::getline(ss, tmp, '.');
	first_nb_ = convertInNb(tmp);

	ss >> tmp;

	sec_nb_ = convertInNb(tmp);

	isValid();
}

void
Version::isValid() const
{
	if (first_nb_ != 1 || sec_nb_ > 1)
		throw Error::ErrorException(505);
}

int
Version::convertInNb(std::string const& buffer)
{
	std::string::const_iterator it;
	std::stringstream			ss;
	int							nb = 0;

	for (it = buffer.begin(); it != buffer.end(); it++)
	{
		if (!isdigit(*it))
			throw Error::ErrorException(400);
	}

	ss << buffer;
	ss >> nb;

	return (nb);
}

Version&
Version::operator=(Version const& to_copy)
{
	if (this != &to_copy)
	{
		protocol_ = to_copy.protocol_;
		first_nb_ = to_copy.first_nb_;
		sec_nb_ = to_copy.sec_nb_;
	}
	return *this;
}

std::ostream&
operator<<(std::ostream& os, Version const& m)
{
	os << m.getProtocol() << "/" << m.getFirstNb() << "." << m.getSecNb();
	return os;
}

Version::~Version()
{}
