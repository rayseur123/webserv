#include "AResponse.hpp"
#include "Request.hpp"

Request const&	AResponse::getRequest() const
{
	return (request_);
}

std::string const&	AResponse::getRequestLine() const
{
	return (request_line_);
}

std::map<std::string, std::string> const&	AResponse::getHeaderMap() const
{
	return (header_map_);
}

std::string const&	AResponse::getBody() const
{
	return (body_);
}

AResponse::AResponse()
{}

AResponse::AResponse(Request const& request, std::string const& request_line,
				std::map<std::string, std::string> const& header_map, std::string const& body)
: request_(request),
  request_line_(request_line),
  header_map_(header_map),
  body_(body)
{}

AResponse::AResponse(AResponse const& to_copy)
: request_(to_copy.request_),
  request_line_(to_copy.request_line_),
  header_map_(to_copy.header_map_),
  body_(to_copy.body_)
{}

AResponse::~AResponse()
{}

AResponse const&	AResponse::operator=(AResponse const& to_copy)
{
	if (&to_copy == this)
		return (*this);
	request_ = to_copy.request_;
	request_line_ = to_copy.request_line_;
	header_map_ = to_copy.header_map_;
	body_ = to_copy.body_;
	return (*this);

}