#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <map>

class Headers
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

	Headers& operator=(Headers const& to_copy);

	Headers();
	Headers(Headers const& to_copy);
	~Headers();
};

std::ostream& operator<<(std::ostream& os, Headers const& m);

#endif
