#include "http/Code.hpp"
#include <cstdarg>

int
Code::getCode() const
{
	return code_;
}

std::string const&
Code::getMessage() const
{
	return message_;
}

std::string const&
Code::getPage() const
{
	return page_;
}

void
Code::setPage(std::string const& page)
{
	page_ = page;
}

void
Code::setMessage(std::string const& message)
{
	message_ = message;
}

void
Code::setCode(int code)
{
	code_ = code;
}

Code::Code() : code_(0)
{}

Code::Code(Code const& to_copy) :
	code_(to_copy.code_), message_(to_copy.message_), page_(to_copy.page_)
{}

Code&
Code::operator=(Code const& to_copy)
{
	if (this == &to_copy)
		return *this;
	code_ = to_copy.code_;
	message_ = to_copy.message_;
	page_ = to_copy.message_;
	return *this;
}

Code::Code(int nb) : code_(nb) {};

Code::~Code()
{}
