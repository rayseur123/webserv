#include <cstring>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "algorithm"

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

std::string
build_error_response(int error_code)
{
	std::string ret;
	switch (error_code)
	{
		case 400:
			ret = "HTTP/1.0 400 Bad Request\r\n\r\n";
			break;
		case 401:
			ret = "HTTP/1.0 401 Unauthorized\r\n\r\n";
			break;
		case 402:
			ret = "HTTP/1.0 402 Payment Required\r\n\r\n";
			break;
		case 403:
			ret = "HTTP/1.0 403 Forbidden\r\n\r\n";
			break;
		case 404:
			ret = "HTTP/1.0 404 Not Found\r\n\r\n";
			break;
		case 405:
			ret = "HTTP/1.0 405 Method Not Allowed\r\n\r\n";
			break;
		case 406:
			ret = "HTTP/1.0 406 Not Acceptable\r\n\r\n";
			break;
		case 407:
			ret = "HTTP/1.0 407 Proxy Authentication Required\r\n\r\n";
			break;
		case 408:
			ret = "HTTP/1.0 408 Request Timeout\r\n\r\n";
			break;
		case 409:
			ret = "HTTP/1.0 409 Conflict\r\n\r\n";
			break;
		case 410:
			ret = "HTTP/1.0 410 Gone\r\n\r\n";
			break;
		case 411:
			ret = "HTTP/1.0 411 Length Required\r\n\r\n";
			break;
		case 412:
			ret = "HTTP/1.0 412 Precondition Failed\r\n\r\n";
			break;
		case 413:
			ret = "HTTP/1.0 413 Payload Too Large\r\n\r\n";
			break;
		case 414:
			ret = "HTTP/1.0 414 URI Too Long\r\n\r\n";
			break;
		case 415:
			ret = "HTTP/1.0 415 Unsupported Media Type\r\n\r\n";
			break;
		case 416:
			ret = "HTTP/1.0 416 Range Not Satisfiable\r\n\r\n";
			break;
		case 417:
			ret = "HTTP/1.0 417 Expectation Failed\r\n\r\n";
			break;
		case 422:
			ret = "HTTP/1.0 422 Unprocessable Content\r\n\r\n";
			break;
		case 500:
			ret = "HTTP/1.0 500 Internal Server Error\r\n\r\n";
			break;
		case 501:
			ret = "HTTP/1.0 501 Not Implemented\r\n\r\n";
			break;
		case 502:
			ret = "HTTP/1.0 502 Bad Gateway\r\n\r\n";
			break;
		case 503:
			ret = "HTTP/1.0 503 Service Unavailable\r\n\r\n";
			break;
		case 504:
			ret = "HTTP/1.0 504 Gateway Timeout\r\n\r\n";
			break;
		case 505:
			ret = "HTTP/1.0 505 HTTP Version Not Supported\r\n\r\n";
			break;
		default:
			ret = "HTTP/1.0 500 Internal Server Error\r\n\r\n";
			break;
	}
	return (ret);
}
