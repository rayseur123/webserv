#include "ResponseGet.hpp"
#include "Request.hpp"
#include <string>
#include "Location.hpp"
#include "AResponse.hpp"

std::string	ResponseGet::buildResponseStr(std::vector<Location> const& locations_vec) const
{
	Location const& location = getGoodLocation(locations_vec);
	std::cout << location.getPath() << std::endl;
	//std::cout << location;
	std::string	ret;
	return (ret);
}

ResponseGet::ResponseGet()
{}

ResponseGet::ResponseGet(Request const& request)
: AResponse(request)
{}

ResponseGet::ResponseGet(ResponseGet const& to_copy)
: AResponse(to_copy)
{}


ResponseGet::~ResponseGet()
{}

ResponseGet&	ResponseGet::operator=(ResponseGet const& to_copy)
{
	if (this == &to_copy)
		return (*this);
	request_ = to_copy.request_;
	request_line_ = to_copy.request_line_;
	header_map_ = to_copy.header_map_;
	body_ = to_copy.body_;
	return (*this);
}