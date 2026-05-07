#include "http/ResponsePost.hpp"
#include "http/AResponse.hpp"
#include "http/parsing/Request.hpp"
#include "parsing/Location.hpp"
#include "utils/utils.hpp"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <linux/close_range.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#define POST_CHECKER (1u << 1u)
#define CHMOD		 0644

namespace
{
	std::string
	generate_filename()
	{
		static bool		  initialized = false;
		std::stringstream ss;
		unsigned long	  timestamp = 0;
		unsigned int	  random_part = 0;

		if (!initialized)
		{
			std::srand(static_cast<unsigned int>(std::clock()));
			initialized = true;
		}

		timestamp = static_cast<unsigned long>(std::time(NULL));
		random_part = static_cast<unsigned int>(std::rand() % 0xFFFF);

		ss << "upload_" << timestamp << "_" << std::hex << random_part
		   << ".tmp";
		return (ss.str());
	}

} // namespace

std::string
ResponsePost::buildResponse(std::vector<Location> const& locations_vec)
{
	Location const& location = getGoodLocation(locations_vec);
	std::string		file_path;

	if (!location.checkAllowMethods(POST_CHECKER))
		return (build_error_response(400));

	file_path = location.buildPathPost(request_);
	int fd = open(file_path.c_str(), O_DIRECTORY | O_CLOEXEC);
	if (fd != -1 && location.getAutoIndex())
	{
		file_path += "/" + generate_filename();
		close(open(file_path.c_str(), O_CREAT | O_CLOEXEC, CHMOD));
		error_code_ = 201;
	}
	close(fd);
	std::ofstream file(file_path.c_str());
	if (!file.is_open())
		return (build_error_response(404));
	file << request_.getBody().getContent();

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
