#include "socket/SocketCgi.hpp"

SocketCgi::SocketCgi(Connection& connection, int fd, pid_t pid) :
	connection_(connection), fd_(fd), pid_child_(pid)
{}

int
SocketCgi::handleEvent(EpollManager& manager, uint32_t events)
{}

SocketCgi::~SocketCgi()
{}
