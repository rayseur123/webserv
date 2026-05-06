#include "http/ResponsePost.hpp"
#include "http/AResponse.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"

#include <cstdio>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#define POST_CHECKER (1u << 1u)
#define CHMOD		 0644

std::string
ResponsePost::buildResponse(std::vector<Location> const& locations_vec)
{
	// Le deplacer car present dans les 3 methods
	Location const& location = getGoodLocation(locations_vec);
	std::string		file_path;
	std::string		body;

	if (location.checkAllowMethods(POST_CHECKER) == 1)
		throw std::logic_error("400");

	file_path = location.buildPath(request_);
	if (!location.getIndex().empty())
		file_path += location.getIndex();
	// jusque ici
	int fd = open(file_path.c_str(), O_DIRECTORY | O_CLOEXEC);
	if (fd != -1 && location.getAutoIndex()) // its a folder
	{
		int fd = open(file_path.c_str(),
					  O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, CHMOD);
		(void) fd;
		std::ofstream new_file(file_path.c_str());
		error_code_ = 201;
	}
	else
	{}
	close(fd);
	setBody(body, file_path);
	return (buildResponseStr());
}

ResponsePost::ResponsePost()
{}

ResponsePost::ResponsePost(Request const& request) : AResponse(request)
{}

ResponsePost::ResponsePost(ResponsePost const& to_copy) : AResponse(to_copy)
{}

ResponsePost::~ResponsePost()
{}

ResponsePost&
ResponsePost::operator=(ResponsePost const& to_copy)
{
	if (this == &to_copy)
		return (*this);
	request_ = to_copy.request_;
	status_line_ = to_copy.status_line_;
	header_vec_ = to_copy.header_vec_;
	body_ = to_copy.body_;
	return (*this);
}
