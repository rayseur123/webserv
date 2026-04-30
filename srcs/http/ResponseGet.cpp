#include "http/ResponseGet.hpp"
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

#define GET_CHECKER (1u << 0)

std::string
ResponseGet::buildResponseStr(std::vector<Location> const& locations_vec) const
{
	Location const& location = getGoodLocation(locations_vec);
	std::string		file_path;
	std::string		body;

	unsigned int allow_method = location.getAllowMethods();
	if ((allow_method & GET_CHECKER) == 0)
		throw std::logic_error("400");
	file_path = location.getRoot() + request_.getUri().getTarget();

	int fd = open(file_path.c_str(), O_DIRECTORY | O_CLOEXEC);
	if (fd != -1 && location.getAutoIndex())
	{
		std::cout << "test" << '\n';
		DIR* dir = opendir(file_path.c_str());
		if (dir == NULL)
			throw std::logic_error("404 file not find1");
		// envoyer le dir content
	}
	else if (fd != -1 && !location.getIndex().empty())
	{

		file_path += location.getIndex();
		std::ifstream file(file_path.c_str());
		if (!file.is_open())
			throw std::logic_error("404 file not find2");
		std::stringstream sstr;
		sstr << file.rdbuf();
		body = sstr.str();
	}
	else
	{
		std::ifstream file(file_path.c_str());
		if (!file.is_open())
			throw std::logic_error("404 file not find3");
		std::stringstream sstr;
		sstr << file.rdbuf();
		body = sstr.str();
	}
	std::string		  ret("HTTP/1.0 200 OK\r\n");
	std::stringstream ss;
	ss << body.length();
	ret += "Content-Length: ";
	ret += ss.str();
	ret += "\r\n\r\n";
	ret += body;
	return (ret);
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
	request_line_ = to_copy.request_line_;
	header_map_ = to_copy.header_map_;
	body_ = to_copy.body_;
	return (*this);
}
