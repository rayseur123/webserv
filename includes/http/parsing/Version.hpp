#ifndef VERSION_HPP
#define VERSION_HPP

#include <iostream>

#define VERSION_LENGTH 8

class Version
{
private:
	std::string protocol_;
	int			first_nb_;
	int			sec_nb_;

	static int convertInNb(std::string const& buffer);
	void	   isValid() const;

public:
	std::string getProtocol() const;
	int			getFirstNb() const;
	int			getSecNb() const;

	void setProtocol(std::string const& protocol);
	void setFirstNb(int nb);
	void setSecNb(int nb);

	bool operator==(Version const& m) const;
	bool operator!=(Version const& m) const;
	bool operator<=(Version const& m) const;

	Version();
	explicit Version(std::string version);
	Version(Version const& to_copy);
	Version& operator=(Version const& to_copy);
	~Version();
};

std::ostream& operator<<(std::ostream& os, Version const& m);

#endif
