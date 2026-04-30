#ifndef URI_HPP
#define URI_HPP

#include <iostream>

class Uri
{
private:
	std::string target_;
	std::string query_;

public:
	static bool isValid(std::string const& target);

	std::string const& getTarget() const;
	std::string const& getQuery() const;

	void setTarget(std::string const& target);
	void setQuery(std::string const& query);

	Uri& operator=(Uri const& to_copy);

	Uri();
	explicit Uri(std::string const& uri);
	Uri(Uri const& to_copy);
	~Uri();
};

std::ostream& operator<<(std::ostream& os, Uri const& m);

#endif
