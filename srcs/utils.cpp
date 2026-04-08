#include <errno.h>
#include <cstring>
#include <iostream>

std::string messageError(std::string const& function_name)
{
	std::string msg;

	msg.append("[Error] ");
	msg.append(function_name);
	msg.append(": ");
	msg.append(strerror(errno));

	return (msg);
}