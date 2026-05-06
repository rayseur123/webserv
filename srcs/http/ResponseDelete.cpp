#include "http/ResponseDelete.hpp"
#include "http/AResponse.hpp"
#include "http/parsing/Body.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"
#include "utils/utils.hpp"

#include <cstdio>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include "utils/utils.hpp"

#define DELETE_CHECKER (1u << 2u)
#define CHMOD		   0644

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
ResponseDelete::buildResponse(std::vector<Location> const& locations_vec)
{
	Location const& location = getGoodLocation(locations_vec);
	std::string		file_path;

	std::cout << "INSIDE THE DELETE RESPONSE" << std::endl;
	if (location.checkAllowMethods(DELETE_CHECKER) != 0)
	{
		return (build_error_response(400));
	}

	file_path = location.buildPath(request_);

	std::string body;
	// TU FAIS CE QUE TU VEUX ICI
	int fd = open(file_path.c_str(), O_DIRECTORY | O_CLOEXEC);
	if (fd != -1) // its a folder
		return (build_error_response(422));

	std::ifstream file(file_path.c_str());

	body = readFileContent(file);

	// Delete part
	if (std::remove(file_path.c_str()))
		return (build_error_response(404));

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
