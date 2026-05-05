#include <dirent.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "http/AResponse.hpp"
#include "http/parsing/Request.hpp"

namespace
{
	std::string
	to_string(int value)
	{
		std::ostringstream oss;
		oss << value;
		return (oss.str());
	}
} // namespace

Location const&
AResponse::getGoodLocation(std::vector<Location> const& locations_vec) const
{
	std::vector<Location>::const_iterator it;
	std::vector<Location>::const_iterator ret;
	std::string							  uri = request_.getUri().getTarget();
	int									  best_value = 0;

	for (it = locations_vec.begin(); it != locations_vec.end(); ++it)
	{
		int actual_value = it->getValue(uri);
		if (actual_value > best_value)
		{
			best_value = actual_value;
			ret = it;
		}
	}
	if (best_value == 0)
		throw std::logic_error("404");
	return (*ret);
}

std::string
AResponse::generateAutoIndex(std::string const& path, std::string const& uri)
{
	DIR* dir = opendir(path.c_str());
	if (dir == NULL)
		throw std::logic_error("404");
	std::string	   ret(uri + "<br><br>");
	struct dirent* content = readdir(dir);
	while (content != NULL)
	{
		ret += "- ";
		ret += content->d_name;
		ret += "<br>";
		content = readdir(dir);
	}
	return (ret);
}

void
AResponse::setBody(std::string const& body)
{
	body_ = body;

	std::stringstream ss;
	ss << body.length();

	bool							   foundLen = false;
	bool							   foundType = false;
	std::vector<std::string>::iterator it;

	for (it = header_vec_.begin(); it != header_vec_.end(); it++)
	{
		if (it->find("Content-Length:") != std::string::npos)
		{
			*it = "Content-Length: " + ss.str();
			foundLen = true;
		}
		if (it->find("Content-Type:") != std::string::npos)
		{
			foundType = true;
		}
	}
	if (!foundLen)
		header_vec_.push_back("Content-Length: " + ss.str());
	if (!foundType)
		header_vec_.push_back("Content-Type: text/html");
}

void
AResponse::addHeader(std::pair<std::string, std::string> const& new_header)
{
	std::string temp;

	temp = new_header.first + ": " + new_header.second;
	header_vec_.push_back(temp);
}

void
AResponse::setResponseCode(int code)
{
	error_code_ = code;
}

Request const&
AResponse::getRequest() const
{
	return (request_);
}

std::string const&
AResponse::getRequestLine() const
{
	return (request_line_);
}

std::vector<std::string> const&
AResponse::getHeaderVec() const
{
	return (header_vec_);
}

std::string const&
AResponse::getBody() const
{
	return (body_);
}

std::string
AResponse::buildRequestLine() const
{
	std::string ret("HTTP/1.0 ");
	ret += to_string(error_code_);
	ret += " OK"; // LIGNE A CHANGER QUAND ON AURA UNE GESTION DS CODES
	return (ret);
}

std::string
AResponse::buildResponseStr() const
{
	std::string ret;

	ret += buildRequestLine();
	ret += "\r\n";

	std::vector<std::string>::const_iterator it;
	for (it = header_vec_.begin(); it != header_vec_.end(); it++)
	{
		ret += *it;
		ret += "\r\n";
	}
	ret += "\r\n";
	ret += body_;
	return (ret);
}

std::string
AResponse::getResponseStr() const
{
	return (buildResponseStr());
}

AResponse::AResponse() : error_code_()
{}

AResponse::AResponse(Request const& request) : request_(request), error_code_()
{}

AResponse::AResponse(Request const& request, std::string const& request_line,
					 std::vector<std::string> const& header_vec,
					 std::string const&				 body) :
	request_(request), request_line_(request_line), header_vec_(header_vec),
	body_(body), error_code_()
{}

AResponse::AResponse(AResponse const& to_copy) :
	request_(to_copy.request_), request_line_(to_copy.request_line_),
	header_vec_(to_copy.header_vec_), body_(to_copy.body_), error_code_()
{}

AResponse::~AResponse()
{}

AResponse&
AResponse::operator=(AResponse const& to_copy)
{
	if (&to_copy == this)
		return (*this);
	request_ = to_copy.request_;
	request_line_ = to_copy.request_line_;
	header_vec_ = to_copy.header_vec_;
	body_ = to_copy.body_;
	return (*this);
}
