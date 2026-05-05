#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>

class Method
{

#define GET	   1
#define POST   2
#define DELETE 4
#define HEAD   8
#define UNLINK 16
#define LINK   32
#define PUT	   64

private:
	int type_;

public:
	int	 getMethod() const;
	void setMethod(int const type);

	bool	operator==(Method const& m) const;
	bool	operator!=(Method const& m) const;
	Method& operator=(Method const& to_copy);

	Method();
	Method(std::string method);
	Method(Method const& to_copy);
	~Method();
};

std::ostream& operator<<(std::ostream& os, Method const& m);

#endif
