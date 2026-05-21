#include "http/ResponseDelete.hpp"
#include "http/AResponse.hpp"
#include "http/httpStatus.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"
#include "utils/utils.hpp"

#include <cstdio>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#define DELETE_CHECKER (1u << 2u)
#define CHMOD		   0644

std::string
ResponseDelete::buildResponse(Location const& location, Listener const& server,
							  std::string const& path)
{
	std::string const& file_path = path;

	if (!location.getRedirect().empty())
		return (buildRedirect(location));

	if (!location.checkAllowMethods(DELETE_CHECKER))
		return (buildErrorResponse(HTTP_METHOD_NOT_ALLOWED, server,
								   request_.getVersion().toString()));

	std::string body;
	int			fd = open(file_path.c_str(), O_DIRECTORY | O_CLOEXEC);
	if (fd != -1)
	{
		close(fd);
		return (buildErrorResponse(HTTP_FORBIDDEN, server,
								   request_.getVersion().toString()));
	}

	std::ifstream file(file_path.c_str());

	body = readFileContent(file);

	if (std::remove(file_path.c_str()) != 0)
		return (buildErrorResponse(HTTP_NOT_FOUND, server,
								   request_.getVersion().toString()));

	error_code_ = HTTP_OK;
	close(fd);
	setBody(body);
	return (buildResponseStr());
}

ResponseDelete::ResponseDelete()
{}

ResponseDelete::ResponseDelete(Request const& request) : AResponse(request)
{}

ResponseDelete::ResponseDelete(ResponseDelete const& to_copy) :
	AResponse(to_copy)
{}

ResponseDelete::~ResponseDelete()
{}

ResponseDelete&
ResponseDelete::operator=(ResponseDelete const& to_copy)
{
	if (this == &to_copy)
		return (*this);
	request_ = to_copy.request_;
	status_line_ = to_copy.status_line_;
	header_vec_ = to_copy.header_vec_;
	body_ = to_copy.body_;
	return (*this);
}
