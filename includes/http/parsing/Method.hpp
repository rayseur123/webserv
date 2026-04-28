#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>

enum
{
	POST,
	GET,
	DELETE,
	HEAD,
	UNLINK,
	LINK,
	PUT
};

class Method
{

private:
	int type_;

public:
	int	 getMethod() const;
	void setMethod(int type);

	bool	operator==(Method const& m) const;
	bool	operator!=(Method const& m) const;
	Method& operator=(Method const& to_copy);

	Method();
	explicit Method(std::string const& method);
	Method(Method const& to_copy);
	~Method();
};

std::ostream& operator<<(std::ostream& os, Method const& m);

#endif
