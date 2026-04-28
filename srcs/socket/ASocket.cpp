#include "socket/ASocket.hpp"

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
{}
