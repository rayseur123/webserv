#ifndef PARSING_REQUEST_HPP
#define PARSING_REQUEST_HPP

#include "http/parsing/Request.hpp"

enum
{
	REQUEST,
	HEADER,
	BODY,
	FINISH
};

enum
{
	UNDEFINED,
	NO_BODY,
	CHUNK_BODY,
	LINE_BODY
};

class ParsingRequest
{
private:
	std::string buffer_;
	Request		request_;
	int			step_;
	int			body_type;

	void defineBodyType();
	void requestLine(std::string& line, size_t pos);
	void headerLine(std::string& line, size_t pos);

public:
	void fillBuffer(std::string& tmp);

	Request& getRequest();
	int		 getStep() const;

	ParsingRequest& operator=(ParsingRequest const& to_copy);

	ParsingRequest();
	ParsingRequest(ParsingRequest const& to_copy);
	~ParsingRequest();
};

#endif
