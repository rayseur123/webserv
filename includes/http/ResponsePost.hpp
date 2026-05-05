#ifndef METHOD_POST
#define METHOD_POST

#include "http/AResponse.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"

class ResponsePost : public AResponse
{
public:
	std::string buildResponse(std::vector<Location> const& locations_vec);

	ResponsePost();
	explicit ResponsePost(Request const& request);
	ResponsePost(ResponsePost const& to_copy);
	~ResponsePost();
	ResponsePost& operator=(ResponsePost const& to_copy);
};

#endif
