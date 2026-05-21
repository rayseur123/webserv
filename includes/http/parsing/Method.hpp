#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>

enum
{
	GET = 1,
	POST = 2,
	DELETE = 4,
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
	int				  getType() const;
	void			  setType(int type);
	std::string const toString() const;

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
