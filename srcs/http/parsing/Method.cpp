#include "http/parsing/Method.hpp"
#include "http/Code.hpp"
#include "http/httpStatus.hpp"

void
Method::setType(int const type)
{
	type_ = type;
}

int
Method::getType() const
{
	return type_;
}

Method&
Method::operator=(Method const& to_copy)
{
	if (this == &to_copy)
		return *this;
	type_ = to_copy.type_;
	return *this;
}

bool
Method::operator==(Method const& m) const
{
	return (type_ == m.type_);
}

bool
Method::operator!=(Method const& m) const
{
	return (type_ != m.type_);
}

Method::Method() : type_(0)
{}

Method::Method(std::string const& method)
{
	if (method == "GET")
		type_ = GET;
	else if (method == "POST")
		type_ = POST;
	else if (method == "DELETE")
		type_ = DELETE;
	else if (method == "HEAD")
		throw Code(HTTP_METHOD_NOT_ALLOWED);
	else if (method == "LINK")
		throw Code(HTTP_METHOD_NOT_ALLOWED);
	else if (method == "UNLINK")
		throw Code(HTTP_METHOD_NOT_ALLOWED);
	else if (method == "PUT")
		throw Code(HTTP_METHOD_NOT_ALLOWED);
	else
		throw Code(HTTP_NOT_IMPLEMENTED);
}

Method::Method(Method const& to_copy) : type_(to_copy.type_)
{}

Method::~Method()
{}

std::string const
Method::toString() const
{
	switch (type_)
	{
		case GET:
			return "GET";

		case POST:
			return "POST";

		case DELETE:
			return "DELETE";
	};
	return "ERROR";
}

std::ostream&
operator<<(std::ostream& os, Method const& m)
{
	switch (m.getType())
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
