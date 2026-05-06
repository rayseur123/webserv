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
	Headers headers_;
	Body	body_;
	int		code_;

public:
	bool bodyIsLength() const;
	bool bodyIsChunked() const;
	void addingInsideHeader(std::pair<std::string, std::string>& head);
	int	 addingBodyLength(std::string& line);
	int	 addingBodyChunked(std::string& container);

	void setCode(int code);
	void setMethod(Method const& method);
	void setMethod(std::string const& method);
	void setUri(Uri const& uri);
	void setUri(std::string const& target);
	void setVersion(Version const& version);
	void setVersion(std::string& version);
	void setHeaders(Headers& type);
	void setHeaders(std::string const& method);
	void setBody(Body const& body);

	void resetRequest();

	int			   getCode();
	Method const&  getMethod() const;
	Uri const&	   getUri() const;
	Version const& getVersion() const;
	Body const&	   getBody() const;
	Headers		   getHeader();

	Request();
	Request(Request const& to_copy);
	Request& operator=(Request const& to_copy);
	~Request();
};

std::ostream& operator<<(std::ostream& os, Request& r);

#endif
