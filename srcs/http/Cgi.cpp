#include "http/Cgi.hpp"
#include <iostream>


void Cgi::buildEnv(Request const& r)
{
	std::string SCRIPT_NAME;
	std::string REQUEST_METHOD;
	std::string GATEWAY_INTERFACE;
	std::string REMOTE_ADDR;
	std::string SERVER_NAME;
	std::string SERVER_PORT;
	std::string SERVER_PROTOCOL;
	std::string QUERY_STRING;
	std::string CONTENT_LENGTH;
	std::string CONTENT_TYPE;
	std::string PATH_INFO;

	SCRIPT_NAME = 
	REQUEST_METHOD = r.getMethod().getType();
	GATEWAY_INTERFACE = "CGI/1.1";
	REMOTE_ADDR =
	SERVER_NAME =
	SERVER_PROTOCOL = r.getVersion().
	QUERY_STRING = r.getUri().getQuery();

	if (REQUEST_METHOD == "POST")
	{
		CONTENT_LENGTH = r.getHeader().get("content-length");

		CONTENT_TYPE = r.getHeader().get("content-type");
		//if (CONTENT_TYPE.length() == 0)
		//{
			//CONTENT_TYPE = 
		//}
	}
	
	//PATH_INFO = 


	std::cout << "\n========== CGI ENVIRONMENT VARIABLES ==========\n\n";
		
	std::cout << "SCRIPT_NAME          : " << SCRIPT_NAME << "\n";
	std::cout << "REQUEST_METHOD       : " << REQUEST_METHOD << "\n";
	std::cout << "GATEWAY_INTERFACE    : " << GATEWAY_INTERFACE << "\n";
	std::cout << "REMOTE_ADDR          : " << REMOTE_ADDR << "\n";
	std::cout << "SERVER_NAME          : " << SERVER_NAME << "\n";
	std::cout << "SERVER_PORT          : " << SERVER_PORT << "\n";
	std::cout << "SERVER_PROTOCOL      : " << SERVER_PROTOCOL << "\n";
	std::cout << "QUERY_STRING         : " << QUERY_STRING << "\n";
	std::cout << "CONTENT_LENGTH       : " << CONTENT_LENGTH << "\n";
	std::cout << "CONTENT_TYPE         : " << CONTENT_TYPE << "\n";
	std::cout << "PATH_INFO            : " << PATH_INFO << "\n";
	
	std::cout << "\n================================================\n" << std::endl;


}




void Cgi::startProgram() const
{}


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
