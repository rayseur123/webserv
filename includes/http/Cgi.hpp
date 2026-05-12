#ifndef CGI_HPP
#define CGI_HPP

#include <string>

class Cgi
{
private:
	std::string env_;

public:
	void buildEnv();
	void startProgram();

	Cgi& operator=(Cgi const& c);

	Cgi();
	Cgi(Cgi const& c);
	~Cgi();
};

#endif
