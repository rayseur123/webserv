#ifndef CODE_HPP
#define CODE_HPP

#include <bits/stdc++.h>


class Code
{
private:
	int			code_;
	std::string message_;
	std::string page_;

public:
	int				   getCode() const;
	std::string const& getMessage() const;
	std::string const& getPage() const;

	void setCode(int code);
	void setMessage(std::string const& message);
	void setPage(std::string const& page);

	Code& operator=(Code const& to_copy);

	Code();
	explicit Code(int nb);
	Code(Code const& to_copy);
	~Code();
};

#endif
