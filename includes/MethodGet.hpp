#ifndef METHOD_GET
#define METHOD_GET

#include "AResponse.hpp"
#include "Location.hpp"

class	MethodGet : public AResponse
{
	public:
		std::string	buildResponseStr(std::vector<Location> const& locations_vec) const;

		MethodGet();
		MethodGet(MethodGet const& to_copy);
		~MethodGet();
		MethodGet const&	operator=(MethodGet const& to_copy);
};

#endif