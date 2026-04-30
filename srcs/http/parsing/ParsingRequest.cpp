#include "http/parsing/ParsingRequest.hpp"
#include <climits>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include "http/Error.hpp"
#include "http/parsing/Body.hpp"
#include "http/parsing/Request.hpp"
#include "utils/utils.hpp"

// Parsing the line of REQUEST
void
ParsingRequest::requestLine(std::string& line, size_t pos)
{
	std::vector<std::string> request_line;

	line.erase(pos, 2);
	request_line = splitLineByDel(line, ' ');

	if (request_line.size() != 3)
		throw Error::ErrorException(400);

	request_.setMethod(request_line[0]);
	request_.setUri(request_line[1]);
	request_.setVersion(request_line[2]);

	buffer_.erase(0, pos + 2);
	step_++;
}

// Parsing lines of HEADERS
void
ParsingRequest::defineBodyType()
{
	Headers tmp = request_.getHeader();

	if (tmp.has("transfer-encoding"))
		body_type = CHUNK_BODY;
	else if (tmp.has("content-type") && tmp.has("content-length"))
		body_type = LINE_BODY;
	else
	{
		body_type = NO_BODY;
		step_++;
	}
}

std::pair<std::string, std::string>
ParsingRequest::splitHeader(std::string& line)
{
	std::pair<std::string, std::string> head;
	size_t								pos = 0;

	pos = line.find(':');
	if (pos == std::string::npos)
		throw(Error::ErrorException(400));

	head.first = line.substr(0, pos);
	head.second = line.substr(pos + 1);

	return head;
}

void
ParsingRequest::headerLine(std::string& line, size_t pos)
{
	std::pair<std::string, std::string> head;

	line.erase(pos, 2);

	head = splitHeader(line);
	toLowerString(head.first);

	if (!keyIsValid(head.first))
		throw Error::ErrorException(400);

	trimSpaceString(head.second);
	request_.addingInsideHeader(head);
	buffer_.erase(0, pos + 2);
}

// Parsing Management

bool
ParsingRequest::handleEndHeaders(std::string const& line)
{
	if (line == "\r\n")
	{
		step_++;
		buffer_.erase(0, 2);
		defineBodyType();
		return true;
	}
	return false;
}

void
ParsingRequest::processBody()
{
	if (step_ == BODY && body_type == LINE_BODY)
		step_ += request_.addingBodyLength(buffer_);
	if (step_ == BODY && body_type == CHUNK_BODY)
		step_ += request_.addingBodyChunked(buffer_);
}

void
ParsingRequest::fillBuffer(std::string& tmp)
{
	size_t		pos = 0;
	std::string line;

	buffer_.append(tmp);
	while (step_ != FINISH)
	{
		pos = buffer_.find("\r\n");
		if (pos == std::string::npos)
			return;

		line = buffer_.substr(0, pos + 2);

		if (step_ == REQUEST)
			requestLine(line, pos);
		else if (step_ == HEADER)
		{
			if (!handleEndHeaders(line))
				headerLine(line, pos);
			if (body_type == LINE_BODY)
				break;
		}
		if (step_ == BODY && body_type == CHUNK_BODY)
		{
			step_ += request_.addingBodyChunked(buffer_);
		}
	}
	if (step_ == BODY && body_type == LINE_BODY)
		step_ += request_.addingBodyLength(buffer_);
}

void
ParsingRequest::resetParsingAndRequest()
{
	step_ = 0;
	body_type = 0;
	request_.resetRequest();
}

int
ParsingRequest::getStep() const
{
	return step_;
}

Request&
ParsingRequest::getRequest()
{
	return request_;
}

ParsingRequest&
ParsingRequest::operator=(ParsingRequest const& to_copy)
{
	if (this == &to_copy)
		return *this;

	buffer_ = to_copy.buffer_;
	request_ = to_copy.request_;
	step_ = to_copy.step_;

	return *this;
}

ParsingRequest::ParsingRequest(ParsingRequest const& to_copy) :
	step_(to_copy.step_), body_type(to_copy.body_type)
{}

ParsingRequest::ParsingRequest() : step_(0), body_type(0)
{}

ParsingRequest::~ParsingRequest()
{}
