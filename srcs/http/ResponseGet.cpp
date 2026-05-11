#include "http/ResponseGet.hpp"
#include "http/AResponse.hpp"
#include "http/httpStatus.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"
#include "utils/utils.hpp"

#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#define GET_CHECKER (1u << 0u)

namespace
{
	std::string
	readFileContent(std::ifstream const& file)
	{
		std::stringstream sstr;
		sstr << file.rdbuf();
		return (sstr.str());
	}
} // namespace

std::string
ResponseGet::buildResponse(std::vector<Location> const& locations_vec)
{
	Location const& location = getGoodLocation(locations_vec);
	std::string		file_path;
	std::string		body;

	if (!location.getRedirect().empty())
		return (buildRedirect(location));

	if (!location.checkAllowMethods(GET_CHECKER))
		return (buildErrorResponse(HTTP_BAD_REQUEST));

	file_path = location.buildPath(request_);

	if (!location.getIndex().empty())
		file_path += location.getIndex();

	int fd = open(file_path.c_str(), O_DIRECTORY | O_CLOEXEC);
	if (fd != -1 && location.getAutoIndex())
	{
		close(fd);
		DIR* dir = opendir(file_path.c_str());
		closedir(dir);
		if (dir == NULL)
			return (buildErrorResponse(HTTP_BAD_REQUEST));
		body = generateAutoIndex(file_path, request_.getUri().getTarget());
	}
	else
	{
		std::ifstream file(file_path.c_str());
		if (!file.is_open())
			return (buildErrorResponse(HTTP_NOT_FOUND));
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
