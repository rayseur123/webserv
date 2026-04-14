#include "Server.hpp"
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <cstdlib> 
#include <iostream>
	
int Server::createSocket()
{
	return (0);
}

void	Server::setLocations(std::vector<Location> const& location_vec)
{
	locations_vec_ = location_vec;
}

void	Server::setMaxClientRequestBody(std::string const& max_client_request_body)
{
	size_t	index = max_client_request_body.find("m");
	max_client_request_body_ = atoi(max_client_request_body.substr(0, index).c_str());
	if (index != std::string::npos)
		max_client_request_body_ *= 1000;
}

void	Server::setAddrAndPort(std::string const& addr_and_port)
{
	size_t	index = addr_and_port.find(":");
	if (index == std::string::npos)
		throw std::invalid_argument("[ERROR] : Invalide port");
	address_ = addr_and_port.substr(0, index);
	port_ = addr_and_port.substr(index + 1);
}

void	Server::setErrorPage(std::vector<std::string> const& error_page)
{
	if (error_page.size() != 3)
		throw std::invalid_argument(" [ERROR] :Invalide error page format");

	error_page_ = std::make_pair(atoi(error_page[1].c_str()), error_page[2]);
}

Server const&   Server::operator=(Server const& to_copy)
{
	if (&to_copy == this)
		return (*this);
	fd_ = to_copy.fd_;
	max_client_request_body_ = to_copy.max_client_request_body_;
	address_ = to_copy.address_;
	port_ = to_copy.port_;
	locations_vec_ = to_copy.locations_vec_;
	error_page_ = to_copy.error_page_;
	return (*this);
}

std::ostream&   operator<<(std::ostream& os, Server const& to_print)
{
	(void)to_print;
	return (os);
}

Server::Server()
	:fd_(-1),
	max_client_request_body_(0)
{}

Server::Server(Server const& to_copy)
	:fd_(to_copy.fd_),
	max_client_request_body_(to_copy.max_client_request_body_),
	address_(to_copy.address_),
	port_(to_copy.port_),
	locations_vec_(to_copy.locations_vec_),
	error_page_(to_copy.error_page_)
{}

Server::Server(int fd, int max_client_request_body, std::string address,
            std::string port, std::vector<Location> locations_vec)
	:fd_(fd),
	max_client_request_body_(max_client_request_body),
	address_(address),
	port_(port),
	locations_vec_(locations_vec)
{}

Server::~Server(){}