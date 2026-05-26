#ifndef CGI_HPP
#define CGI_HPP

#include <exception>
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
	void defineExtension(std::string path);

	void childProcess(int										fds[2],
					  std::map<std::string, std::string> const& path_map,
					  std::string const&						path);

	Cgi& operator=(Cgi const& c);

	Cgi();
	Cgi(Cgi const& c);
	~Cgi();
};

class ForkException : public std::exception
{
	char const* what() const throw();
};

#endif
