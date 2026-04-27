#include <errno.h>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include "algorithm"
#include <vector>


bool	stringIsDigit(std::string s)
{
	std::string::iterator it;
	
	for (it = s.begin(); it != s.end(); it++)
	{
		if (!isdigit(*it))
			return 0;
	}
	return 1;
}

bool	keyIsValid(std::string s)
{
	std::string::iterator it;

	for (it = s.begin(); it != s.end(); it++)
	{
		if (!std::isalpha(*it))
		{
			if (*it != '-')
				return 0;
		}
	}
	return 1;
}

std::vector<std::string> splitLineByDel(std::string line, char del)
{
    std::vector<std::string> tmp;
    std::stringstream ss(line);
    std::string buffer;
    
    while (std::getline(ss, buffer, del))
        tmp.push_back(buffer);
    return (tmp);
}

void    toLowerString(std::string& tmp)
{
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
}

int    stoi(std::string s)
{
    std::stringstream ss;
    int nb;
    
    ss << s;
    ss >> std::hex >> nb;
    
    return nb;
}

std::string messageError(std::string const& function_name)
{
	std::string msg;

	msg.append("[Error] ");
	msg.append(function_name);
	msg.append(": ");
	msg.append(strerror(errno));

	return (msg);
}