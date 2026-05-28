#include "http/ResponseGet.hpp"
#include "http/httpStatus.hpp"
#include "utils/utils.hpp"

#include <dirent.h>

#define GET_CHECKER (1u << 0u)

std::string
ResponseGet::buildResponse(Location const& location, Listener const& server,
						   std::string const& path)
{
	std::string file_path = path;
	std::string body;

	if (!location.getRedirect().empty())
		return (buildRedirect(location));

	if (!location.checkAllowMethods(GET_CHECKER))
		return (buildErrorResponse(HTTP_METHOD_NOT_ALLOWED, server,
								   request_.getVersion().toString()));
	if (!location.getIndex().empty())
		file_path += location.getIndex();

	if (isDir(file_path) && location.getAutoIndex())
	{
		DIR* dir = opendir(file_path.c_str());
		if (dir == NULL && errno == EACCES)
			return (buildErrorResponse(HTTP_FORBIDDEN, server,
									   request_.getVersion().toString()));
		if (dir == NULL)
			return (buildErrorResponse(HTTP_INTERNAL_SERVER_ERROR, server,
									   request_.getVersion().toString()));
		closedir(dir);
		body = generateAutoIndex(file_path, request_.getUri().getTarget());
	}
	else
	{
		std::ifstream file(file_path.c_str());
		if (!file.is_open())
			return (buildErrorResponse(HTTP_NOT_FOUND, server,
									   request_.getVersion().toString()));
		body = readFileContent(file);
	}
	error_code_ = HTTP_OK;
	setBody(body);

	return (buildResponseStr());
}

ResponseGet::ResponseGet()
{}

ResponseGet::ResponseGet(Request const& request) : AResponse(request)
{}

ResponseGet::ResponseGet(ResponseGet const& to_copy) : AResponse(to_copy)
{}

ResponseGet::~ResponseGet()
{}

ResponseGet&
ResponseGet::operator=(ResponseGet const& to_copy)
{
	if (this == &to_copy)
		return (*this);
	request_ = to_copy.request_;
	status_line_ = to_copy.status_line_;
	header_vec_ = to_copy.header_vec_;
	body_ = to_copy.body_;
	return (*this);
}
