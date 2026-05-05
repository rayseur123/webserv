#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>

std::string				 messageError(std::string const& function_name);
bool					 keyIsValid(std::string const& s);
bool					 stringIsDigit(std::string const& s);
void					 toLowerString(std::string& tmp);
std::vector<std::string> splitLineByDel(std::string const& line, char del);
void					 trimSpaceString(std::string& s);

template<typename T>
std::ostream&
operator<<(std::ostream& os, std::vector<T> list)
{
	typename std::vector<T>::iterator it;

	os << "----------------------------" << std::endl;
	for (it = list.begin(); it != list.end(); ++it)
	{
		os << *it;
		os << "----------------------------" << std::endl;
	}
	os << std::endl;
	return os;
}

#endif
