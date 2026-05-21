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
	std::string				 extension_;

public:
	void addingRequestHeaderEnv(Request const& r);
	void buildEnv(Request const& r, Listener const& s,
				  std::string const& addr_client, std::string& path);
	void startProgram(Request const& r, Connection& c, std::string const& path,
					  Location const& location);
	void parseUri(Request const& r, std::string& path);
	bool isCgiProgram(std::string const& s);

	Cgi& operator=(Cgi const& c);

	Cgi();
	Cgi(Cgi const& c);
	~Cgi();
};

#endif
