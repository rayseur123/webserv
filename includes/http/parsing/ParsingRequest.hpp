#ifndef PARSING_REQUEST_HPP
#define PARSING_REQUEST_HPP

#include <utility>
#include "http/parsing/Request.hpp"

enum
{
	UNDEFINED,
	LINE_BODY,
	CHUNK_BODY,
	NO_BODY,
	TRAILER
};

enum
{
	REQUEST,
	HEADER,
	BODY,
	FINISH
};

class ParsingRequest
{

private:
	size_t		max_body_length_;
	std::string buffer_;
	Request		request_;
	int			step_;
	int			body_type;
	int			code_;

	std::pair<std::string, std::string> splitHeader(std::string& line);
	bool handleEndHeaders(std::string const& line);

	void processBody();
	void defineBodyType();
	void requestLine(std::string& line, size_t pos);
	void headerLine(std::string& line, size_t pos);

	void handleBodyLine();
	bool handleBodyChunk();

public:
	void fillBuffer(std::string& tmp);

	Request& getRequest();
	int		 getStep() const;
	int		 getCode() const;
	size_t	 getMaxBodyLength() const;

	void setCode(int nb);
	void setMaxBodyLength(size_t nb);

	ParsingRequest& operator=(ParsingRequest const& to_copy);

	ParsingRequest();
	ParsingRequest(ParsingRequest const& to_copy);
	~ParsingRequest();
};

#endif
