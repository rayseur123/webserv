#include "Listener.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include "utils.hpp"
#include "Connection.hpp"

void	Listener::acceptNewConnection(EpollManager &manager)
{
	epoll_event ev;

    while (true)
    {
        sockaddr_in connection_addr = {};
        socklen_t size = sizeof(connection_addr);

        int connection_fd = accept(fd_, reinterpret_cast<sockaddr *>(&connection_addr), &size);
        
        if (connection_fd == -1)
            break;
        
        ev.data.fd = connection_fd;
        ev.events = EVENTS_CONNECTION;
        
        if (epoll_ctl(manager.getEpollFd(), EPOLL_CTL_ADD, connection_fd, &ev) == -1)
            throw std::logic_error(messageError("acceptNewClient>epoll_ctl"));
		Connection* connection = new Connection(connection_fd, *this);
		manager.addConnection(std::make_pair(connection_fd, connection));
    }
}

int	    Listener::handleEvent(EpollManager &manager, int events)
{
	if (events & (EPOLLERR | EPOLLRDHUP))
		return (1);
	else if (events & EPOLLIN)
		acceptNewConnection(manager);
    return (0);

	
	return (0);
}

void	Listener::setNoBlockingFd()
{
	int flags = fcntl(fd_, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error(messageError("createSocket>flags_fcntl"));

	if (fcntl(fd_, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error(messageError("createSocket>set_fcntl"));
}

int     Listener::createListenerSocket()
{
	int option = 1;
	addrinfo hints = {};
	hints.ai_family = AF_UNSPEC; // IPv4 / IPv6
	hints.ai_socktype = SOCK_STREAM; // Communication par flux de bytes, avec des données hors bande pour les données prioritaires
	hints.ai_flags = AI_PASSIVE; // si node (address_.c_str()) est NULL, alors getaddrinfo trouvera quand même une adresse disponible

	addrinfo *res = NULL;

	if (getaddrinfo(address_.c_str(), port_.c_str(), &hints, &res) != 0)
		throw std::runtime_error(messageError("createSocket>getaddrinfo"));

	addrinfo *addr_try;
	for (addr_try = res; addr_try != NULL; addr_try = addr_try->ai_next) 
	{
		fd_ = socket(addr_try->ai_family, addr_try->ai_socktype, addr_try->ai_protocol);
		if (fd_ == -1)
			continue;

		setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

		if (bind(fd_, addr_try->ai_addr, addr_try->ai_addrlen) == 0)
			break;

		close(fd_);
		fd_ = -1;
	}

	freeaddrinfo(res);

	if (fd_ == -1)
		throw std::runtime_error(messageError("createSocket>bind failed for all addresses"));

	setNoBlockingFd();
		
	if (listen(fd_, LISTEN_QUEUE) == -1)
		throw std::runtime_error(messageError("createSocket>listen"));
	
	return (fd_);
}

void	Listener::setLocations(std::vector<Location> const& location_vec)
{
	locations_vec_ = location_vec;
}

void	Listener::setMaxClientRequestBody(std::string const& max_client_request_body)
{
	size_t	index = max_client_request_body.find("m");
	max_client_request_body_ = atoi(max_client_request_body.substr(0, index).c_str());
	if (index != std::string::npos)
		max_client_request_body_ *= 1000;
}

void	Listener::setAddrAndPort(std::string const& addr_and_port)
{
	size_t	index = addr_and_port.find(":");
	if (index == std::string::npos)
		throw std::invalid_argument("[ERROR] : Invalide port");
	address_ = addr_and_port.substr(0, index);
	port_ = addr_and_port.substr(index + 1);
}

void	Listener::setErrorPage(std::vector<std::string> const& error_page)
{
	if (error_page.size() != 3)
		throw std::invalid_argument(" [ERROR] :Invalide error page format");

	error_page_ = std::make_pair(atoi(error_page[1].c_str()), error_page[2]);
}

std::string const& Listener::getPort() const
{
	return (port_);
}

int	Listener::getMaxClientRequestBody() const
{
	return (max_client_request_body_);
}

std::string const&	Listener::getAddress() const
{
	return (address_);
}

std::vector<Location> const&	Listener::getLocations() const
{
	return (locations_vec_);
}

std::pair<int, std::string> const&	Listener::getErrorPage() const
{
	return (error_page_);
}

Listener&   Listener::operator=(Listener const& to_copy)
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

Listener::Listener()
	:ASocket(-1),
	max_client_request_body_(0)
{}

Listener::Listener(Listener const& to_copy)
	:ASocket(to_copy.fd_),
	max_client_request_body_(to_copy.max_client_request_body_),
	address_(to_copy.address_),
	port_(to_copy.port_),
	locations_vec_(to_copy.locations_vec_),
	error_page_(to_copy.error_page_)
{}

Listener::Listener(int fd, int max_client_request_body, std::string const& address,
            std::string const& port, std::vector<Location> const& locations_vec)
	:ASocket(fd),
	max_client_request_body_(max_client_request_body),
	address_(address),
	port_(port),
	locations_vec_(locations_vec)
{}

Listener::~Listener(){}

std::ostream& operator<<(std::ostream& os, Listener const& to_print)
{
    os << "SERVER [" << to_print.getAddress() << ":" << to_print.getPort() << "]" << std::endl;
    os << "\tclient_max_body_size: " << to_print.getMaxClientRequestBody() << std::endl;
    
    std::pair<int, std::string> err = to_print.getErrorPage();
    if (err.first != 0) {
        os << "\terror_page: " << err.first << " -> " << err.second << std::endl;
    }

    std::vector<Location> const& locs = to_print.getLocations();
    for (size_t i = 0; i < locs.size(); ++i) {
        os << locs[i];
    }

    os << "--------------------------------------" << std::endl;
    return (os);
}