#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "ASocket.hpp"
#include <vector>
#include <string>
#include <utility>
#include "Location.hpp"
#include "EpollManager.hpp"

#define LISTEN_QUEUE 10

class Listener : public ASocket
{
	private:
		int                         max_client_request_body_;
        std::string                 address_;
        std::string                 port_;
        std::vector<Location>       locations_vec_;
        std::pair<int, std::string> error_page_;
	public:
        void	acceptNewConnection(EpollManager &manager);
        void	setNoBlockingFd();
        int     createListenerSocket();
        int	    handleEvent(EpollManager &manager, int events);

		int                                 getMaxClientRequestBody() const; 
        std::string const&                  getPort() const;
        std::string const&                  getAddress() const;
        std::vector<Location> const&        getLocations() const;
        std::pair<int, std::string> const&  getErrorPage() const;

		void	setLocations(std::vector<Location> const& location_vec);
        void	setMaxClientRequestBody(std::string const& max_client_request_body);
        void	setAddrAndPort(std::string const& addr_and_port);
        void	setErrorPage(std::vector<std::string> const& error_page);

		Listener();
		Listener(Listener const& to_copy);
		Listener(int fd, int max_client_request_body, std::string const& address,
            std::string const& port, std::vector<Location> const& locations_vec);
		Listener&   operator=(Listener const& to_copy);
		~Listener();
};

std::ostream&   operator<<(std::ostream& os, Listener const& to_print);

#endif
