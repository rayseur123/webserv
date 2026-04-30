#include "http/parsing/Uri.hpp"
#include "http/Error.hpp"

void
Uri::setTarget(std::string const& target)
{
	if (isValid(target))
		target_ = target;
}

void
Uri::setQuery(std::string const& query)
{
	query_ = query;
}

bool
Uri::isValid(std::string const& target)
{
	return (*target.begin() == '/');
}

std::string const&
Uri::getTarget() const
{
	return target_;
}

std::string const&
Uri::getQuery() const
{
	return query_;
}

Uri&
Uri::operator=(Uri const& to_copy)
{
	if (this != &to_copy)
	{
		target_ = to_copy.target_;
		query_ = to_copy.query_;
	}
	return *this;
}

Uri::Uri()
{}

Uri::Uri(std::string const& uri)
{
	size_t pos = 0;
	if (!isValid(uri))
		throw Error::ErrorException(400);

	pos = uri.find('?');
	if (pos != std::string::npos)
	{
		target_ = uri.substr(0, pos);
		query_ = uri.substr(pos + 1);
	}
	else
	{
		target_ = uri;
	}
}

Uri::Uri(Uri const& to_copy) : target_(to_copy.target_), query_(to_copy.query_)
{}

Uri::~Uri()
{}

std::ostream&
operator<<(std::ostream& os, Uri const& m)
{
	os << m.getTarget();
	os << " |?| ";
	os << m.getQuery();
	return os;
}
