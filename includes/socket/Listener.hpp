#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "epoll/EpollManager.hpp"
#include "parsing/Location.hpp"
#include "socket/ASocket.hpp"

#define LISTEN_QUEUE 10
#define BASE		 10

class Listener : public ASocket
{
private:
	size_t									max_client_request_body_;
	std::string								address_;
	std::string								port_;
	std::vector<Location>					locations_vec_;
	std::map<std::vector<int>, std::string> error_page_;

	Listener& operator=(Listener const& to_copy);
	Listener(Listener const& to_copy);

public:
	void acceptNewConnection(EpollManager& manager);
	void setNoBlockingFd();
	int	 createListenerSocket();
	int	 handleEvent(EpollManager& manager, uint32_t events);

	size_t						 getMaxClientRequestBody() const;
	std::string const&			 getPort() const;
	std::string const&			 getAddress() const;
	std::vector<Location> const& getLocations() const;
	std::map<std::vector<int>, std::string> const& getErrorPage() const;

	void setLocations(std::vector<Location> const& location_vec);
	bool setMaxClientRequestBody(std::string const& max_client_request_body);
	bool setAddrAndPort(std::string const& addr_and_port);
	bool setErrorPage(std::vector<std::string> const& error_page);

	Listener();
	Listener(int fd, int max_client_request_body, std::string const& address,
			 std::string const&			  port,
			 std::vector<Location> const& locations_vec);
	~Listener();
};

std::ostream& operator<<(std::ostream& os, Listener const& to_print);

#endif
