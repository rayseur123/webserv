#include <climits>
#include <cstddef>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "epoll/signal.hpp"
#include "http/Code.hpp"
#include "http/httpStatus.hpp"
#include "http/parsing/ParsingRequest.hpp"
#include "http/parsing/Request.hpp"
#include "utils/utils.hpp"

// Request line
void
ParsingRequest::requestLine(std::string& line, size_t pos)
{
	std::vector<std::string> request_line;

	line.erase(pos, 2);
	request_line = splitLineByDel(line, ' ');

	try
	{
		if (request_line.size() != 3)
			throw Code(HTTP_BAD_REQUEST);
		request_.setMethod(request_line[0]);
		request_.setUri(request_line[1]);
		request_.setVersion(request_line[2]);
	}
	catch (Code& c)
	{
		code_ = c.getCode();
	}

	buffer_.erase(0, pos + 2);
	step_++;
}

// Headers line
void
ParsingRequest::defineBodyType()
{
	Headers tmp = request_.getHeader();

	if (tmp.has("transfer-encoding"))
		body_type = CHUNK_BODY;
	else if (tmp.has("content-length"))
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
		throw Code(HTTP_BAD_REQUEST);

	head.first = line.substr(0, pos);
	head.second = line.substr(pos + 1);

	return head;
}

void
ParsingRequest::headerLine(std::string& line, size_t pos)
{
	std::pair<std::string, std::string> head;

	line.erase(pos, 2);

	try
	{
		head = splitHeader(line);
		toLowerString(head.first);

		if (!keyIsValid(head.first))
			throw Code(HTTP_BAD_REQUEST);

		trimSpaceString(head.second);
		request_.addingInsideHeader(head);
	}
	catch (Code& c)
	{
		code_ = c.getCode();
	}
	buffer_.erase(0, pos + 2);
}

// Parsing Management

bool
ParsingRequest::handleEndHeaders(std::string const& line)
{
	if (line == "\r\n")
	{
		buffer_.erase(0, 2);
		if (body_type == TRAILER)
		{
			step_ = FINISH;

			return true;
		}
		step_++;
		defineBodyType();
		return true;
	}
	return false;
}

void
ParsingRequest::fillBuffer(std::string& tmp)
{
	size_t		pos = 0;
	std::string line;

	buffer_.append(tmp);
	while (step_ != FINISH && body_type != LINE_BODY)
	{

		if (Signal::signal == 1)
			throw(SIGINT);

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
			if (handleBodyChunk())
			{
				return;
			}
	}
	if (step_ == BODY && body_type == LINE_BODY)
		handleBodyLine();
}

// Body line
bool
ParsingRequest::handleBodyChunk()
{
	try
	{
		step_ += request_.addingBodyChunked(buffer_);

		if (request_.getBody().getContent().length() > max_body_length_)
			throw(Code(HTTP_PAYLOAD_TOO_LARGE));

		if (step_ == FINISH)
		{
			step_ = HEADER;
			body_type = TRAILER;
		}
		return false;
	}
	catch (Code& c)
	{
		code_ = c.getCode();
		step_ = FINISH;
		return true;
	}
}

void
ParsingRequest::handleBodyLine()
{

	if (request_.getHeader().getContentLength() > max_body_length_)
	{
		code_ = HTTP_PAYLOAD_TOO_LARGE;
		step_ = FINISH;
		return;
	}

	step_ += request_.addingBodyLength(buffer_);
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

int
ParsingRequest::getCode() const
{
	return code_;
}

size_t
ParsingRequest::getMaxBodyLength() const
{
	return max_body_length_;
}

void
ParsingRequest::setCode(int nb)
{
	code_ = nb;
}

void
ParsingRequest::setMaxBodyLength(size_t nb)
{
	max_body_length_ = nb;
}

ParsingRequest&
ParsingRequest::operator=(ParsingRequest const& to_copy)
{
	if (this == &to_copy)
		return *this;

	buffer_ = to_copy.buffer_;
	request_ = to_copy.request_;
	body_type = to_copy.body_type;
	step_ = to_copy.step_;
	code_ = to_copy.code_;

	return *this;
}

ParsingRequest::ParsingRequest(ParsingRequest const& to_copy) :
	step_(to_copy.step_), body_type(to_copy.body_type), code_(0)
{}

ParsingRequest::ParsingRequest() : step_(0), body_type(0), code_(0)
{}

ParsingRequest::~ParsingRequest()
{}
