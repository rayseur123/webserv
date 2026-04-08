#include "Server.hpp"

int Server::createSocket()
{
	return (0);
}

Server const&   Server::operator=(Server const& to_copy)
{
	if (&to_copy == this)
		return (*this);
	fd_ = to_copy.fd_;
	max_client_request_body_ = to_copy.max_client_request_body_;
	address_ = to_copy.address_;
	locations_vec_ = to_copy.locations_vec_;
	return (*this);
}

Server::Server()
: fd_(0), max_client_request_body_(0) 
{}

Server::Server(Server const& to_copy)
: fd_(to_copy.fd_), max_client_request_body_(to_copy.max_client_request_body_),
  address_(to_copy.address_), locations_vec_(to_copy.locations_vec_)
{}

Server::Server(int fd, int max_client_request_body, std::string address,
            std::vector<Location> locations_vec)
: fd_(fd), max_client_request_body_(max_client_request_body),
  address_(address), locations_vec_(locations_vec)
{}

Server::~Server(){}