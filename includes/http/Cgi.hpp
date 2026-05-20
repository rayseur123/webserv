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
	std::string				 path_;

public:
	void addingRequestHeaderEnv(Request const& r);
	void buildEnv(Request const& r, Listener const& s,
				  std::string const& addr_client);
	void startProgram(Request const& r, Connection& c, std::string const& path,
					  Location const& location) const;
	void parseUri(Request const& r);
	void createPath(std::string const& target);

	Cgi& operator=(Cgi const& c);

	Cgi();
	Cgi(Cgi const& c);
	~Cgi();
};

#endif
