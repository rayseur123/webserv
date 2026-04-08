#include "Server.hpp"
#include <utils.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

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

int const& Server::getPort() const
{
	return (port_);
}

std::vector<Location> const& Server::getLocation() const
{
	return (locations_vec_);
}

#include <netdb.h>

void Server::createSocket()
{

	addrinfo hints;
	addrinfo * result, rp;

	sockaddr_in	socket_addr = {};

	socket_addr.sin_addr.s_addr = address_;
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(port_);

	fd_ = socket(AF_INET, SOCK_STREAM, 1);
	if (!fd_)
		throw(std::range_error(messageError("createSocket>socket")));
	
	if (bind(fd_, reinterpret_cast<sockaddr *>(&socket_addr), sizeof(socket_addr)) == -1)
		throw(std::range_error(messageError("createSocket>bind")));
	
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