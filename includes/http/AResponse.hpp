#ifndef ARESPONSE_HPP
#define ARESPONSE_HPP

#include <string>
#include <utility>

#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"

class AResponse
{
protected:
	Request request_;

	std::string				 status_line_;
	std::vector<std::string> header_vec_;
	std::string				 body_;
	int						 error_code_;

public:
	Location const&
	getGoodLocation(std::vector<Location> const& locations_vec) const;

	std::string generateAutoIndex(std::string const& path,
								  std::string const& uri);
	std::string buildRedirect(Location const& location);

	std::string buildRequestLine() const;
	std::string buildResponseStr() const;

	virtual std::string
	buildResponse(std::vector<Location> const& locations_vec) = 0;

	void setBody(std::string const& body);
	void addHeader(std::pair<std::string, std::string> const& new_header);
	void setResponseCode(int code);

	Request const&					getRequest() const;
	std::string const&				getRequestLine() const;
	std::vector<std::string> const& getHeaderVec() const;
	std::string const&				getBody() const;
	std::string						getResponseStr() const;

	AResponse();
	explicit AResponse(Request const& request);
	AResponse(Request const& request, std::string const& status_line,
			  std::vector<std::string> const& header_vec,
			  std::string const&			  body);
	AResponse(AResponse const& to_copy);
	virtual ~AResponse();

	AResponse& operator=(AResponse const& to_copy);
};

#endif
