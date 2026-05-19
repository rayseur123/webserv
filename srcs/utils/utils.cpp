#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "http/httpStatus.hpp"
#include "http/parsing/Request.hpp"
#include "socket/Listener.hpp"

#define DECIMAL 10

std::string
readFileContent(std::ifstream const& file)
{
	std::stringstream sstr;
	sstr << file.rdbuf();
	return (sstr.str());
}

bool
stringIsDigit(std::string const& s)
{
	std::string::const_iterator it;

	for (it = s.begin(); it != s.end(); it++)
	{
		if (!isdigit(*it))
			return false;
	}
	return true;
}

std::string
digitToString(int nb)
{
	std::stringstream ss;

	ss << nb;

	return ss.str();
}

std::string
inet_ntop(char* adress)
{
	std::string tmp;

	tmp += digitToString(static_cast<int>(adress[0]));
	tmp += '.';
	tmp += digitToString(static_cast<int>(adress[1]));
	tmp += '.';
	tmp += digitToString(static_cast<int>(adress[2]));
	tmp += '.';
	tmp += digitToString(static_cast<int>(adress[3]));

	return tmp;
}

bool
keyIsValid(std::string const& s)
{
	std::string::const_iterator it;

	for (it = s.begin(); it != s.end(); it++)
	{
		if (!isalpha(*it))
		{
			if (*it != '-')
				return false;
		}
	}
	return true;
}

void
trimSpaceString(std::string& s)
{
	s.erase(0, s.find_first_not_of(' '));
}

std::vector<std::string>
splitLineByDel(std::string const& line, char del)
{
	std::vector<std::string> tmp;
	std::stringstream		 ss(line);
	std::string				 buffer;

	while (std::getline(ss, buffer, del))
		tmp.push_back(buffer);
	return (tmp);
}

void
toLowerString(std::string& tmp)
{
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
}

void
toUpperString(std::string& tmp)
{
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper);
}

std::string
messageError(std::string const& function_name)
{
	std::string msg;

	msg.append("[Error] ");
	msg.append(function_name);
	msg.append(": ");
	msg.append(strerror(errno));

	return (msg);
}

std::string
getStatusMessage(int code)
{
	switch (code)
	{
		case HTTP_OK:
			return "200 OK";
		case HTTP_CREATED:
			return "201 Created";
		case HTTP_MOVED_PERMANENTLY:
			return "301 Moved Permanently";
		case HTTP_BAD_REQUEST:
			return "400 Bad Request";

		case HTTP_UNAUTHORIZED:
			return "401 Unauthorized";

		case HTTP_PAYMENT_REQUIRED:
			return "402 Payment Required";

		case HTTP_FORBIDDEN:
			return "403 Forbidden";

		case HTTP_NOT_FOUND:
			return "404 Not Found";

		case HTTP_METHOD_NOT_ALLOWED:
			return "405 Method Not Allowed";

		case HTTP_NOT_ACCEPTABLE:
			return "406 Not Acceptable";

		case HTTP_PROXY_AUTH_REQUIRED:
			return "407 Proxy Authentication Required";

		case HTTP_REQUEST_TIMEOUT:
			return "408 Request Timeout";

		case HTTP_CONFLICT:
			return "409 Conflict";

		case HTTP_GONE:
			return "410 Gone";

		case HTTP_LENGTH_REQUIRED:
			return "411 Length Required";

		case HTTP_PRECONDITION_FAILED:
			return "412 Precondition Failed";

		case HTTP_PAYLOAD_TOO_LARGE:
			return "413 Payload Too Large";

		case HTTP_URI_TOO_LONG:
			return "414 URI Too Long";

		case HTTP_UNSUPPORTED_MEDIA_TYPE:
			return "415 Unsupported Media Type";

		case HTTP_RANGE_NOT_SATISFIABLE:
			return "416 Range Not Satisfiable";

		case HTTP_EXPECTATION_FAILED:
			return "417 Expectation Failed";

		case HTTP_INTERNAL_SERVER_ERROR:
			return "500 Internal Server Error";

		case HTTP_NOT_IMPLEMENTED:
			return "501 Not Implemented";

		case HTTP_BAD_GATEWAY:
			return "502 Bad Gateway";

		case HTTP_SERVICE_UNAVAILABLE:
			return "503 Service Unavailable";

		case HTTP_GATEWAY_TIMEOUT:
			return "504 Gateway Timeout";

		case HTTP_HTTP_VERSION_NOT_SUPPORTED:
			return "505 HTTP Version Not Supported";

		default:
			return "500 Internal Server Error";
	}
}

std::string
buildErrorResponse(int code, Listener const& server, std::string const& version)
{
	std::map<std::vector<int>, std::string> const& map = server.getErrorPage();
	std::map<std::vector<int>, std::string>::const_iterator it;
	std::string ret("HTTP/1.0 " + getStatusMessage(code));

	for (it = map.begin(); it != map.end(); ++it)
	{
		std::vector<int>::const_iterator it_vec =
			std::find(it->first.begin(), it->first.end(), code);

		if (it->first.empty() || it_vec == it->first.end())
			continue;

		std::ifstream file(it->second.c_str());
		if (!file.is_open())
			return (version + " " +
					getStatusMessage(HTTP_INTERNAL_SERVER_ERROR) +
					"\r\nContent-length:0\r\n\r\n");

		std::string		  file_content(readFileContent(file));
		std::stringstream ss;

		ret += "\r\n";
		ret += "Content-Length: ";
		ss << file_content.length();
		ret += ss.str();
		ret += "\r\n\r\n";
		ret += file_content;
		return (ret);
	}

	return (version + " " + getStatusMessage(code));
}

std::string
makeCodeResponse(int code, std::string const& version)
{
	return (version + " " + getStatusMessage(code));
}

Location const&
getGoodLocation(std::vector<Location> const& locations_vec,
				Request const&				 request)
{
	std::vector<Location>::const_iterator it;
	std::vector<Location>::const_iterator ret;
	std::string							  uri = request.getUri().getTarget();
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
