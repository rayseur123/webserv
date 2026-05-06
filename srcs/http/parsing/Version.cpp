#include "http/parsing/Version.hpp"
#include <cstdlib>
#include "http/Code.hpp"
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

Version::Version() : first_nb_(-1), sec_nb_(-1)
{}

Version::Version(std::string version)
{

	if (version.length() < 8)
		throw Code(400);

	// Protocol
	std::string tmp;
	size_t		pos = 0;

	pos = version.find('/');
	if (pos == std::string::npos)
		throw Code(400);

	tmp = version.substr(0, pos);
	if (tmp != "HTTP")
		throw Code(400);

	protocol_ = tmp;

	version.erase(0, pos + 1);

	// Version
	pos = version.find('.');
	first_nb_ = convertInNb(version.substr(0, pos));
	version.erase(0, pos + 1);
	sec_nb_ = convertInNb(version);

	isValid();
}

void
Version::isValid() const
{
	if (first_nb_ == 1)
		if (sec_nb_ == 0 || sec_nb_ == 1)
			return;
	throw Code(505);
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
			throw Code(400);
	}

	ss << buffer;
	ss >> nb;

	return (nb);
}

Version&
Version::operator=(Version const& to_copy)
{
	if (this == &to_copy)
		return *this;
	protocol_ = to_copy.protocol_;
	first_nb_ = to_copy.first_nb_;
	sec_nb_ = to_copy.sec_nb_;
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
