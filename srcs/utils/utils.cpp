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
		case 100:
			ret = "100 Continue";
			break;
		case 101:
			ret = "101 Switching Protocols";
			break;
		case 200:
			ret = "200 OK";
			break;
		case 201:
			ret = "201 Created";
			break;
		case 202:
			ret = "202 Accepted";
			break;
		case 203:
			ret = "203 Non-Authoritative Information";
			break;
		case 204:
			ret = "204 No Content";
			break;
		case 205:
			ret = "205 Reset Content";
			break;
		case 206:
			ret = "206 Partial Content";
			break;
		case 300:
			ret = "300 Multiple Choices";
			break;
		case 301:
			ret = "301 Moved Permanently";
			break;
		case 302:
			ret = "302 Found";
			break;
		case 303:
			ret = "303 See Other";
			break;
		case 304:
			ret = "304 Not Modified";
			break;
		case 305:
			ret = "305 Use Proxy";
			break;
		case 307:
			ret = "307 Temporary Redirect";
			break;
		case 400:
			ret = "400 Bad Request";
			break;
		case 401:
			ret = "401 Unauthorized";
			break;
		case 402:
			ret = "402 Payment Required";
			break;
		case 403:
			ret = "403 Forbidden";
			break;
		case 404:
			ret = "404 Not Found";
			break;
		case 405:
			ret = "405 Method Not Allowed";
			break;
		case 406:
			ret = "406 Not Acceptable";
			break;
		case 407:
			ret = "407 Proxy Authentication Required";
			break;
		case 408:
			ret = "408 Request Timeout";
			break;
		case 409:
			ret = "409 Conflict";
			break;
		case 410:
			ret = "410 Gone";
			break;
		case 411:
			ret = "411 Length Required";
			break;
		case 412:
			ret = "412 Precondition Failed";
			break;
		case 413:
			ret = "413 Payload Too Large";
			break;
		case 414:
			ret = "414 URI Too Long";
			break;
		case 415:
			ret = "415 Unsupported Media Type";
			break;
		case 416:
			ret = "416 Range Not Satisfiable";
			break;
		case 417:
			ret = "417 Expectation Failed";
			break;
		case 500:
			ret = "500 Internal Server Error";
			break;
		case 501:
			ret = "501 Not Implemented";
			break;
		case 502:
			ret = "502 Bad Gateway";
			break;
		case 503:
			ret = "503 Service Unavailable";
			break;
		case 504:
			ret = "504 Gateway Timeout";
			break;
		case 505:
			ret = "505 HTTP Version Not Supported";
			break;
		default:
			ret = "500 Internal Server Error";
			break;
	}
	return (ret);
}
