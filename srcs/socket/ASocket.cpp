#include "socket/ASocket.hpp"
#include <unistd.h>

int
ASocket::getFd() const
{
	return (fd_);
}

ASocket::ASocket() : fd_(-1)
{}

ASocket::ASocket(int fd) : fd_(fd)
{}

ASocket::~ASocket()
{
	if (fd_ > 0)
		close(fd_);
}
