#ifndef URI_HPP
#define URI_HPP

#include <iostream>

class Uri
{
private:
	std::string target_;

public:
	static bool isValid(std::string const& target);

	std::string const& getTarget() const;
	void			   setTarget(std::string const& target);

	Uri& operator=(Uri const& to_copy);

	Uri();
	explicit Uri(std::string const& uri);
	Uri(Uri const& to_copy);
	~Uri();
};

std::ostream& operator<<(std::ostream& os, Uri const& m);

#endif
