#ifndef ASOCKET_HPP
#define ASOCKET_HPP

#include <stdint.h>

class EpollManager;

class ASocket
{
protected:
	int fd_;

public:
	int			getFd() const;
	virtual int handleEvent(EpollManager& manager, uint32_t events) = 0;

	ASocket();
	ASocket(int fd);
	virtual ~ASocket();
};

#endif
