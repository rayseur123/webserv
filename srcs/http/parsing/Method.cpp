#include "http/parsing/Method.hpp"
#include "http/Error.hpp"

void
Method::setMethod(int const type)
{
	type_ = type;
}

int
Method::getMethod() const
{
	return type_;
}

Method&
Method::operator=(Method const& to_copy)
{
	if (this != &to_copy)
		type_ = to_copy.type_;
	return *this;
}

bool
Method::operator==(Method const& m) const
{
	if (type_ == m.type_)
		return 1;
	return 0;
}

bool
Method::operator!=(Method const& m) const
{
	if (type_ != m.type_)
		return 1;
	return 0;
}

Method::Method() : type_(0)
{}

Method::Method(std::string method)
{
	if (method == "GET")
		type_ = GET;
	else if (method == "POST")
	{
		type_ = POST;
	}
	else if (method == "DELETE")
		type_ = DELETE;
	else if (method == "HEAD")
		throw Error::ErrorException(405);
	else if (method == "LINK")
		throw Error::ErrorException(405);
	else if (method == "UNLINK")
		throw Error::ErrorException(405);
	else if (method == "PUT")
		throw Error::ErrorException(405);
	else
		throw Error::ErrorException(501);
}

Method::Method(Method const& to_copy)
{
	*this = to_copy;
}

Method::~Method()
{}

std::ostream&
operator<<(std::ostream& os, Method const& m)
{
	switch (m.getMethod())
	{
		case GET:
			os << "GET";
			break;

		case POST:
			os << "POST";
			break;

		case DELETE:
			os << "DELETE";
			break;
	}
	return os;
}
