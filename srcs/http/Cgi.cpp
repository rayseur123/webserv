#include "http/Cgi.hpp"


void Cgi::buildEnv()
{

}

void Cgi::startProgram()
{
    
}


Cgi::Cgi(Cgi const& c)
{
	env_ = c.env_;
}

Cgi&
Cgi::operator=(Cgi const& c)
{
	if (this == &c)
		env_ = c.env_;
	return *this;
}

Cgi::Cgi() {};

Cgi::~Cgi() {};
