#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <vector>
#include "http/parsing/Body.hpp"
#include "http/parsing/Header.hpp"
#include "http/parsing/Method.hpp"
#include "http/parsing/Uri.hpp"
#include "http/parsing/Version.hpp"

class Request
{
private:
	Method	method_;
	Uri		uri_;
	Version version_;
	Header	header_;
	Body	body_;

public:
	bool bodyIsLength() const;
	bool bodyIsChunked() const;
	void addingInsideHeader(std::vector<std::string>& param);
	int	 addingBodyLength(std::string& line);
	int	 addingBodyChunked(std::string& container);

	void setMethod(Method const& method);
	void setUri(Uri const& uri);
	void setVersion(Version const& version);
	void setHeader(Header& type);
	void setBody(Body const& body);

	Method const&  getMethod() const;
	Uri const&	   getUri() const;
	Version const& getVersion() const;
	Body const&	   getBody() const;
	Header		   getHeader();

	Request();
	Request(Request const& to_copy);
	Request& operator=(Request const& to_copy);
	~Request();
};

std::ostream& operator<<(std::ostream& os, Request& r);

#endif
