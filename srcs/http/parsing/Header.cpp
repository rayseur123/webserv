#include "http/parsing/Header.hpp"
#include <sstream>
#include "http/Code.hpp"
#include "http/httpStatus.hpp"
#include "utils/utils.hpp"

void
Headers::set(std::string const& key, std::string& value)
{
	if (has(key))
		throw(Code(HTTP_BAD_REQUEST));
	if (key == "content-length")
		if (!stringIsDigit(value))
			throw(Code(HTTP_BAD_REQUEST));
	if (key == "transfer-encoding")
		if (value != "chunked")
			throw(Code(HTTP_BAD_REQUEST));
	headers_[key] = value;
}

std::string
Headers::get(std::string const& key) const
{
	std::map<std::string, std::string>::const_iterator it;
	std::string const								   empty;

	it = headers_.find(key);

	if (it == headers_.end())
		return empty;
	return it->second;
}

bool
Headers::has(std::string const& key) const
{
	std::map<std::string, std::string>::const_iterator it;

	it = headers_.find(key);
	return (it != headers_.end());
}

size_t
Headers::getContentLength() const
{
	std::stringstream ss;
	size_t			  number = 0;

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
