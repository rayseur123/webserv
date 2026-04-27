#include "MethodGet.hpp"
#include "Request.hpp"
#include <string>
#include "Location.hpp"

std::string	MethodGet::buildResponseStr(std::vector<Location> const& locations_vec) const
{
	(void)locations_vec;
	std::string	ret;
	return (ret);
}

MethodGet::MethodGet()
{}

MethodGet::MethodGet(MethodGet const& to_copy)
: AResponse(to_copy)
{}


MethodGet::~MethodGet()
{}

MethodGet const&	MethodGet::operator=(MethodGet const& to_copy)
{
	if (this == &to_copy)
		return (*this);
	request_ = to_copy.request_;
	request_line_ = to_copy.request_line_;
	header_map_ = to_copy.header_map_;
	body_ = to_copy.body_;
	return (*this);
}