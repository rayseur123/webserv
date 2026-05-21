#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <map>

class Headers
{
private:
	std::map<std::string, std::string> headers_;

public:
	void		set(std::string const& key, std::string& value);
	std::string get(std::string const& key) const;

	std::map<std::string, std::string> const& getHeaders() const;
	bool									  has(std::string const& key) const;

	size_t getContentLength() const;

	Headers& operator=(Headers const& to_copy);

	Headers();
	Headers(Headers const& to_copy);
	~Headers();
};

std::ostream& operator<<(std::ostream& os, Headers const& m);

#endif
