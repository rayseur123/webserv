#include "http/ResponseDelete.hpp"
#include "http/AResponse.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"
#include "utils/utils.hpp"

#include <cstdio>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#define DELETE_CHECKER (1u << 2u)
#define CHMOD		   0644

std::string
ResponseDelete::buildResponse(std::vector<Location> const& locations_vec)
{
	Location const& location = getGoodLocation(locations_vec);
	std::string		file_path;

	if (location.checkAllowMethods(DELETE_CHECKER) == 1)
		return (build_error_response(400));

	file_path = location.buildPath(request_);

	// TU FAIS CE QUE TU VEUX ICI
	int fd = open(file_path.c_str(), O_DIRECTORY | O_CLOEXEC);
	if (fd != -1 && location.getAutoIndex()) // its a folder
	{
		// si c'est un dossier
	}
	else
	{
		// sinon
	}
	close(fd);
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
