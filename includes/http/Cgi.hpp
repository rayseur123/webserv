#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include "http/parsing/Request.hpp"

class Cgi
{
private:
	std::string env_;

public:
	void buildEnv(Request const& r);
	void startProgram() const;

	Cgi& operator=(Cgi const& c);

	Cgi();
	Cgi(Cgi const& c);
	~Cgi();
};

#endif
