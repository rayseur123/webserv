#ifndef BODY_HPP
#define BODY_HPP

#include <cstddef>
#include <iostream>

enum
{
	GETTING_LENGTH,
	READING,
};

class Body
{
private:
	int			status_;
	size_t		writed_;
	size_t		length_;
	std::string content_;

public:
	int chunkedBody(std::string& container);
	int lengthBody(std::string& line);

	std::string const& getContent() const;
	size_t			   getLength() const;
	size_t			   getWrited() const;

	void setLength(int nb);
	void setContent(std::string const& content);
	void setWrited(int nb);

	Body();
	explicit Body(std::string const& content);
	Body(Body const& to_copy);
	Body& operator=(Body const& to_copy);
	~Body();
};

std::ostream& operator<<(std::ostream& os, Body const& m);

#endif
