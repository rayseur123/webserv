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
		if (!std::isalpha(*it))
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
