#ifndef SOCKETCGI_HPP
#define SOCKETCGI_HPP

#include "socket/ASocket.hpp"
#include "socket/Connection.hpp"

#include <csignal>
#include <stdint.h>
#include <string>

class Connection;
class EpollManager;

class SocketCgi : public ASocket
{
private:
	Connection& connection_;
	int			fd_;
	pid_t		pid_child_;
	std::string response_;

	SocketCgi();
	SocketCgi(SocketCgi const& to_copy);
	SocketCgi& operator=(SocketCgi const& to_copy);

public:
	explicit SocketCgi(Connection const& connection);
	virtual int handleEvent(EpollManager& manager, uint32_t events);
};

std::ostream& operator<<(std::ostream& os, SocketCgi const& socketCgi);

#endif
