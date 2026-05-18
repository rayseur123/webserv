#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "epoll/EpollManager.hpp"
#include "http/parsing/ParsingRequest.hpp"
#include "socket/ASocket.hpp"

#include <stdint.h>

class Listener;
class EpollManager;

class Connection : public ASocket
{
private:
	Listener const& server_;
	EpollManager&	manager_;
	ParsingRequest	parsing_request_;
	std::string		addr_client_;
	Connection(Connection const& to_copy);
	Connection& operator=(Connection const& to_copy);

public:
	int handleConnectionRequest();

	bool bodyLengthValid();

	int sendMsg(std::string const& msg);

	void handleCGI(Request const& request, std::string& response_str);
	void handleHTTP(Request const& request, std::string& response_str);

	virtual int		   handleEvent(EpollManager& manager, uint32_t events);
	Listener const&	   getServer() const;
	std::string const& getClientAddr() const;

	Connection(int fd, Listener& server, std::string addr,
			   EpollManager& manager);
	~Connection();
};

std::ostream& operator<<(std::ostream& os, Connection const& connection);

#endif
