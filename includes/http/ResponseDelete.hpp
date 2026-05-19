#ifndef METHOD_DELETE
#define METHOD_DELETE

#include "http/AResponse.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"

class ResponseDelete : public AResponse
{
public:
	std::string buildResponse(Location const& location, Listener const& server,
							  std::string const& path);

	ResponseDelete();
	explicit ResponseDelete(Request const& request);
	ResponseDelete(ResponseDelete const& to_copy);
	~ResponseDelete();
	ResponseDelete& operator=(ResponseDelete const& to_copy);
};

#endif
