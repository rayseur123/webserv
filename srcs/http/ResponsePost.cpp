#include "http/ResponsePost.hpp"
#include "http/httpStatus.hpp"
#include "utils/utils.hpp"

#include <cstdlib>
#include <ctime>
#include <sstream>

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
ResponsePost::buildResponse(Location const& location, Listener const& server,
							std::string const& path)
{
	std::string file_path = path;
	if (!location.getRedirect().empty())
		return (buildRedirect(location));

	if (!location.checkAllowMethods(POST_CHECKER))
		return (buildErrorResponse(HTTP_METHOD_NOT_ALLOWED, server,
								   request_.getVersion().toString()));

	if (location.getUploadStore().empty())
		return (buildErrorResponse(HTTP_FORBIDDEN, server,
								   request_.getVersion().toString()));
	file_path = location.buildPathPost(request_);

	if (isDir(file_path))
	{
		std::string file_path_temp = file_path + "/" + generate_filename();
		while (fileExists(file_path_temp))
			file_path_temp = file_path + "/" + generate_filename();
		file_path = file_path_temp;
		error_code_ = HTTP_CREATED;
	}
	std::ofstream file(file_path.c_str());
	if (file.fail())
	{
		if (errno == EACCES)
			return (buildErrorResponse(HTTP_FORBIDDEN, server,
									   request_.getVersion().toString()));
		if (errno == ENOENT)
			return (buildErrorResponse(HTTP_NOT_FOUND, server,
									   request_.getVersion().toString()));
		return (buildErrorResponse(HTTP_INTERNAL_SERVER_ERROR, server,
								   request_.getVersion().toString()));
	}
	file << request_.getBody().getContent();
	if (error_code_ == 0)
		error_code_ = HTTP_OK;
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
