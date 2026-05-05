#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "http/parsing/ParsingRequest.hpp"
#include "socket/ASocket.hpp"

class Listener;
class EpollManager;

class Connection : public ASocket
{
private:
	Listener const& server_;
	ParsingRequest	parsing_request_;
	int code;

public:
	int handleConnectionRequest();

	virtual int		handleEvent(EpollManager& manager, int events);
	Listener const& getServer() const;

	void setMaxClientRequestBody();

	Connection(int fd, Listener& server);
	Connection(Connection const& to_copy);
	Connection& operator=(Connection const& to_copy);
	~Connection();
};

std::ostream& operator<<(std::ostream& os, Connection const& connection);

#endif
