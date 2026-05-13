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
	void addingRequestHeaderEnv(Request const& r);
	void buildEnv(Request const& r, Listener const& s,
				  std::string const& addr_client);
	void startProgram() const;
	void parseUri(Request const& r);

	Cgi& operator=(Cgi const& c);

	Cgi();
	Cgi(Cgi const& c);
	~Cgi();
};

// int a = 234234234;
// char *b = &a;
// b[0];
// b[1];
// b[2];
// b[3]

// 00000000 00000000 00000000 00000000

#endif
