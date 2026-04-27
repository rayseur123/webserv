#ifndef METHOD_GET
#define METHOD_GET

#include "AResponse.hpp"
#include "Location.hpp"
#include "Request.hpp"

class	ResponseGet : public AResponse
{
	public:
		std::string	buildResponseStr(std::vector<Location> const& locations_vec) const;

		ResponseGet();
		ResponseGet(Request const& request);
		ResponseGet(ResponseGet const& to_copy);
		~ResponseGet();
		ResponseGet&	operator=(ResponseGet const& to_copy);
};

#endif