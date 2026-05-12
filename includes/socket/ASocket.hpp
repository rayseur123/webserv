#ifndef ASOCKET_HPP
#define ASOCKET_HPP

#include <stdint.h>

class EpollManager;

class ASocket
{
protected:
	int fd_;

	ASocket(ASocket const& to_copy);
	ASocket& operator=(ASocket const& to_copy);

public:
	int			getFd() const;
	virtual int handleEvent(EpollManager& manager, uint32_t events) = 0;

	ASocket();
	explicit ASocket(int fd);
	virtual ~ASocket();
};

#endif
