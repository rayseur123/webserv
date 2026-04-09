#include "Server.hpp"
#include <utils.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netdb.h>


Server& findServerByFd(std::vector<Server> &server_vec, int& server_fd)
{
	for(size_t i = 0; i < server_vec.size(); i++)
	{
		if (server_vec[i].getFd() == server_fd)
			return (server_vec[i]);
	}
	throw(std::logic_error("[Error] findServerByFd: Server not find !"));
}

int const& Server::getFd() const
{
	return (fd_);
}

int const& Server::getMaxClientRequestBody() const
{
	return (max_client_request_body_);
}
std::string const& Server::getPort() const
{
	return (port_);
}

std::vector<Location> const& Server::getLocation() const
{
	return (locations_vec_);
}

void Server::createSocket()
{

	addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;

	addrinfo *res = NULL;

	if (getaddrinfo(address_.c_str(), port_.c_str(), &hints, &res) != 0)
		throw(std::range_error(messageError("createSocket>getaddrinfo")));

	fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd_ == -1)
		throw(std::range_error(messageError("createSocket>socket")));
	
	if (bind(fd_, res->ai_addr, res->ai_addrlen) == -1)
		throw(std::range_error(messageError("createSocket>bind")));
	
	freeaddrinfo(res);
	
	if (listen(fd_, LISTEN_QUEUE) == -1)
		throw(std::range_error(messageError("createSocket>listen")));

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
: max_client_request_body_(0) 
{}

Server::Server(Server const& to_copy)
: fd_(to_copy.fd_), max_client_request_body_(to_copy.max_client_request_body_),
  address_(to_copy.address_), locations_vec_(to_copy.locations_vec_)
{}

Server::Server(int max_client_request_body, std::string port,std::string address,
            std::vector<Location>& locations_vec)
: max_client_request_body_(max_client_request_body), port_(port),
  address_(address), locations_vec_(locations_vec)
{}

Server::~Server(){}