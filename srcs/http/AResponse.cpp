#include "http/AResponse.hpp"
#include <stdexcept>
#include <string>
#include <vector>
#include "http/parsing/Request.hpp"

Location const&
AResponse::getGoodLocation(std::vector<Location> const& locations_vec) const
{
	std::vector<Location>::const_iterator it;
	std::vector<Location>::const_iterator ret;
	std::string							  uri = request_.getUri().getTarget();
	int									  best_value = 0;

	for (it = locations_vec.begin(); it != locations_vec.end(); ++it)
	{
		int actual_value = it->getValue(uri);
		if (actual_value > best_value)
		{
			best_value = actual_value;
			ret = it;
		}
	}
	std::cout << best_value << std::endl;
	if (best_value == 0)
		throw std::logic_error("404");
	return (*ret);
}

Request const&
AResponse::getRequest() const
{
	return (request_);
}

std::string const&
AResponse::getRequestLine() const
{
	return (request_line_);
}

std::map<std::string, std::string> const&
AResponse::getHeaderMap() const
{
	return (header_map_);
}

std::string const&
AResponse::getBody() const
{
	return (body_);
}

AResponse::AResponse()
{}

AResponse::AResponse(Request const& request) : request_(request)
{}

AResponse::AResponse(Request const& request, std::string const& request_line,
					 std::map<std::string, std::string> const& header_map,
					 std::string const&						   body) :
	request_(request), request_line_(request_line), header_map_(header_map),
	body_(body)
{}

AResponse::AResponse(AResponse const& to_copy) :
	request_(to_copy.request_), request_line_(to_copy.request_line_),
	header_map_(to_copy.header_map_), body_(to_copy.body_)
{}

AResponse::~AResponse()
{}

AResponse&
AResponse::operator=(AResponse const& to_copy)
{
	if (&to_copy == this)
		return (*this);
	request_ = to_copy.request_;
	request_line_ = to_copy.request_line_;
	header_map_ = to_copy.header_map_;
	body_ = to_copy.body_;
	return (*this);
}
