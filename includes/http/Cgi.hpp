#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>

#include "http/parsing/Request.hpp"
#include "socket/Listener.hpp"

class Cgi
{
private:
	std::vector<std::string> env_;

public:
	void buildEnv(Request const& r, Listener const& s);
	void startProgram() const;

	Cgi& operator=(Cgi const& c);

	Cgi();
	Cgi(Cgi const& c);
	~Cgi();
};

#endif
