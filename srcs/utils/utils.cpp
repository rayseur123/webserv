#include <cstring>
#include <errno.h>
#include <sstream>
#include <string>
#include <vector>

#include "algorithm"
#include "http/httpStatus.hpp"

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

	// size_t i = 0;

	// while (i < 3)
	// {
	// 	size_t pos = 0;

	// 	pos = line.find(del)
	// }
}

void
toLowerString(std::string& tmp)
{
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
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

inline char const*
getStatusMessage(int code)
{
	switch (code)
	{
		case HTTP_OK:
			return "200 OK";
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
buildErrorResponse(int code)
{
	return "HTTP/1.0 " + std::string(getStatusMessage(code));
}
