#include "http/parsing/Header.hpp"
#include <sstream>
#include "http/Error.hpp"
#include "utils/utils.hpp"

void
Headers::typeAccepted(std::string& value)
{
	if (value != "text/html")
		value = "application/octet-stream";
}

void
Headers::set(std::string const& key, std::string& value)
{
	if (has(key))
		throw(Error::ErrorException(400));
	if (key == "content-length")
		if (!stringIsDigit(value))
			throw(Error::ErrorException(400));
	if (key == "content-type")
		typeAccepted(value);
	if (key == "transfer-encoding")
		if (value != "chunked")
			throw(Error::ErrorException(400));
	headers_[key] = value;
}

std::string&
Headers::get(std::string const& key)
{
	return headers_[key];
}

bool
Headers::has(std::string const& key) const
{
	std::map<std::string, std::string>::const_iterator it;

	it = headers_.find(key);
	return (it != headers_.end());
}

int
Headers::getContentLength()
{
	std::stringstream ss;
	int				  number = 0;

	ss << get("content-length");
	ss >> number;
	return number;
}

Headers&
Headers::operator=(Headers const& to_copy)
{
	if (&to_copy == this)
		return *this;
	headers_ = to_copy.headers_;
	return *this;
}

Headers::Headers()
{}

Headers::Headers(Headers const& to_copy) : headers_(to_copy.headers_)
{}

Headers::~Headers()
{}

std::map<std::string, std::string> const&
Headers::getHeaders() const
{
	return headers_;
}

std::ostream&
operator<<(std::ostream& os, Headers const& m)
{
	std::map<std::string, std::string>::const_iterator it;

	for (it = m.getHeaders().begin(); it != m.getHeaders().end(); ++it)
	{
		os << it->first;
		os << ":";
		os << it->second << '\n';
	}
	return os;
}
