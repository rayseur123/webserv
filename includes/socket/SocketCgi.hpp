#ifndef SOCKETCGI_HPP
#define SOCKETCGI_HPP

#include "socket/ASocket.hpp"
#include "socket/Connection.hpp"

#include <csignal>
#include <sched.h>
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
	SocketCgi(Connection const& connection, int fd, pid_t pid);
	virtual int handleEvent(EpollManager& manager, uint32_t events);
	~SocketCgi();
};

std::ostream& operator<<(std::ostream& os, SocketCgi const& socketCgi);

#endif
