#ifndef ARESPONSE_HPP
#define ARESPONSE_HPP

#include <map>
#include <string>

#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"

class AResponse
{
protected:
	Request request_;

	std::string						   request_line_;
	std::map<std::string, std::string> header_map_;
	std::string						   body_;

public:
	Location const&
	getGoodLocation(std::vector<Location> const& locations_vec) const;

	virtual std::string
	buildResponseStr(std::vector<Location> const& locations_vec) const = 0;

	Request const&							  getRequest() const;
	std::string const&						  getRequestLine() const;
	std::map<std::string, std::string> const& getHeaderMap() const;
	std::string const&						  getBody() const;

	AResponse();
	AResponse(Request const& request);
	AResponse(Request const& request, std::string const& request_line,
			  std::map<std::string, std::string> const& header_map,
			  std::string const&						body);
	AResponse(AResponse const& to_copy);
	virtual ~AResponse();

	AResponse& operator=(AResponse const& to_copy);
};

#endif
