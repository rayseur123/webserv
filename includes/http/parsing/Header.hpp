#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <map>

class Header
{
private:
	std::map<std::string, std::string> headers_;

public:
	static void typeAccepted(std::string& value);

	void		 set(std::string const& key, std::string& value);
	std::string& get(std::string const& key);
	std::map<std::string, std::string> const& getHeaders() const;
	bool									  has(std::string const& key) const;

	int getContentLength();

	Header& operator=(Header const& to_copy);

	Header();
	Header(Header const& to_copy);
	~Header();
};

std::ostream& operator<<(std::ostream& os, Header const& m);

#endif
