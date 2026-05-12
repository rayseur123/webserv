#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "http/parsing/ParsingRequest.hpp"
#include "socket/ASocket.hpp"

#include <stdint.h>

class Listener;
class EpollManager;

class Connection : public ASocket
{
private:
	Listener const& server_;
	ParsingRequest	parsing_request_;
	Connection(Connection const& to_copy);
	Connection& operator=(Connection const& to_copy);

public:
	int handleConnectionRequest();

	bool bodyLengthValid();

	virtual int		handleEvent(EpollManager& manager, uint32_t events);
	Listener const& getServer() const;

	Connection(int fd, Listener& server);
	~Connection();
};

std::ostream& operator<<(std::ostream& os, Connection const& connection);

#endif
