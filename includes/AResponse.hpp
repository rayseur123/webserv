#ifndef ARESPONSE_HPP
#define ARESPONSE_HPP

#include <string>
#include <map>
#include <vector>
#include "Request.hpp"
#include "Location.hpp"

class	AResponse
{
	protected:
		Request 							request_;
		
		std::string							request_line_;
		std::map<std::string, std::string>	header_map_;
		std::string							body_;
	public:
		virtual std::string							buildResponseStr(std::vector<Location> const& locations_vec) const = 0;

		Request const&								getRequest() const;
		std::string const&							getRequestLine() const;
		std::map<std::string, std::string> const&	getHeaderMap() const;
		std::string const&							getBody() const;

		AResponse();
		AResponse(Request const& request, std::string const& request_line,
				std::map<std::string, std::string> const& header_map, std::string const& body);
		AResponse(AResponse const& to_copy);
		virtual ~AResponse();

		AResponse const&	operator=(AResponse const& to_copy);
};

#endif