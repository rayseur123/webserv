#include "http/parsing/Body.hpp"
#include <cstdlib>
#include <string>
#include "http/Code.hpp"
#include "http/httpStatus.hpp"

int
Body::chunkedBody(std::string& container)
{
	char* end = NULL;
	size_t pos = 0;

	if (status_ == GETTING_LENGTH)
	{
		pos = container.find("\r\n");
		if (pos == std::string::npos)
			return 0;

		length_ =
			std::strtoul(container.substr(0, pos).c_str(), &end, HEXA_BASE);

		container.erase(0, pos + 2);
		if (length_ == 0)
			return 1;

		status_++;
	}
	if (status_ == READING)
	{
		std::string tmp;
		size_t		last = 0;

		if (container.size() < length_ + 2)
			return 0;

		tmp = container.substr(0, length_);
		container.erase(0, length_);

		last = container.find("\r\n");
		if (last != 0)
			throw Code(HTTP_BAD_REQUEST);

		content_ += tmp;
		container.erase(0, 2);
		status_--;
	}
	return 0;
}

int
Body::lengthBody(std::string& line)
{
	std::string tmp;

	if (writed_ < length_)
	{
		tmp = line.substr(0, length_ - writed_);
		content_.append(tmp);
		writed_ += tmp.length();
		line.erase(0, tmp.length() + 2);
	}
	if (writed_ >= length_)
		return 1;

	return 0;
}

void
Body::setLength(int nb)
{
	length_ = nb;
}

void
Body::setContent(std::string const& content)
{
	content_ = content;
}

void
Body::setWrited(int nb)
{
	writed_ = nb;
}

size_t
Body::getLength() const
{
	return length_;
}

std::string const&
Body::getContent() const
{
	return content_;
}

size_t
Body::getWrited() const
{
	return writed_;
}

Body::Body() : status_(0), writed_(0), length_(0)
{}

Body::Body(std::string const& content) :
	status_(0), writed_(0), length_(0), content_(content)
{}

Body&
Body::operator=(Body const& to_copy)
{
	if (this == &to_copy)
		return *this;

	status_ = to_copy.status_;
	writed_ = to_copy.writed_;
	length_ = to_copy.length_;
	content_ = to_copy.content_;
	return *this;
}

Body::Body(Body const& to_copy) :
	status_(to_copy.status_), writed_(to_copy.writed_),
	length_(to_copy.status_), content_(to_copy.content_)
{}

Body::~Body()
{}

std::ostream&
operator<<(std::ostream& os, Body const& m)
{
	os << m.getContent();
	return os;
}
