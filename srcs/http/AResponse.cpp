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

	std::string ret;
	ret += "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
	ret += "<title>Index of " + uri + "</title>";
	ret += "<style>";
	ret += "body{margin:0;background:#0f0f0f;color:#e0e0e0;font-family:'"
		   "Courier New',monospace;}";
	ret += "header{padding:2rem;border-bottom:1px solid #2a2a2a;}";
	ret += "h1{margin:0;font-size:1.1rem;font-weight:normal;color:#888;}";
	ret += "h1 span{color:#fff;}";
	ret += "table{width:100%;border-collapse:collapse;}";
	ret += "tr{border-bottom:1px solid #1a1a1a;transition:background .15s;}";
	ret += "tr:hover{background:#1a1a1a;}";
	ret += "td{padding:.7rem 2rem;font-size:.9rem;}";
	ret += "td:first-child{color:#5b9bd5;}";
	ret += "a{color:inherit;text-decoration:none;}";
	ret += "a:hover{text-decoration:underline;}";
	ret += "</style></head><body>";
	ret += "<header><h1>Index of <span>" + uri + "</span></h1></header>";
	ret += "<table>";

	struct dirent* content = readdir(dir);
	while (content != NULL)
	{
		ret += "<tr><td><a href='";
		ret += uri;
		if (uri[uri.length() - 1] != '/')
			ret += '/';
		ret += content->d_name;
		ret += "'>";
		ret += content->d_name;
		ret += "</a></td></tr>";
		content = readdir(dir);
	}

	ret += "</table></body></html>";
	closedir(dir);
	return (ret);
}

void
AResponse::setBody(std::string const& body)
{
	body_ = body;

	std::stringstream ss;
	ss << body.length();

	bool							   foundLen = false;
	std::vector<std::string>::iterator it;

	for (it = header_vec_.begin(); it != header_vec_.end(); it++)
	{
		if (it->find("Content-Length:") != std::string::npos)
		{
			*it = "Content-Length: " + ss.str();
			foundLen = true;
		}
	}
	if (!foundLen)
		header_vec_.push_back("Content-Length: " + ss.str());
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

std::string
AResponse::findType(std::string const& file_name) const
{
	(void) file_name;
	return ("text/html"); // fonction a changer
}

Request const&
AResponse::getRequest() const
{
	return (request_);
}

std::string const&
AResponse::getRequestLine() const
{
	return (status_line_);
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

AResponse::AResponse(Request const& request, std::string const& status_line,
					 std::vector<std::string> const& header_vec,
					 std::string const&				 body) :
	request_(request), status_line_(status_line), header_vec_(header_vec),
	body_(body), error_code_()
{}

AResponse::AResponse(AResponse const& to_copy) :
	request_(to_copy.request_), status_line_(to_copy.status_line_),
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
	status_line_ = to_copy.status_line_;
	header_vec_ = to_copy.header_vec_;
	body_ = to_copy.body_;
	return (*this);
}
