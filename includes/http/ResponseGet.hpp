#ifndef METHOD_GET
#define METHOD_GET

#include "http/AResponse.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"

class ResponseGet : public AResponse
{
public:
	std::string buildResponse(Location const& location, Listener const& server,
							  std::string const& path);

	ResponseGet();
	explicit ResponseGet(Request const& request);
	ResponseGet(ResponseGet const& to_copy);
	~ResponseGet();
	ResponseGet& operator=(ResponseGet const& to_copy);
};

#endif
