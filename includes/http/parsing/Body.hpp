#ifndef BODY_HPP
#define BODY_HPP

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
	int			writed_;
	int			length_;
	std::string content_;

public:
	int chunkedBody(std::string& container);
	int lengthBody(std::string& container);

	std::string const& getContent() const;
	int				   getLength() const;
	int				   getWrited() const;

	void setLength(int lenght);
	void setContent(std::string const& content);
	void setWrited(int writed);

	Body();
	Body(std::string content);
	Body(Body const& to_copy);
	Body& operator=(Body const& to_copy);
	~Body();
};

std::ostream& operator<<(std::ostream& os, Body const& m);

#endif
