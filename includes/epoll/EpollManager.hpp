#ifndef EPOLL_MANAGER_HPP
#define EPOLL_MANAGER_HPP

#define EVENTS_CONNECTION EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI
#define EVENTS_SERVER	  EPOLLIN | EPOLLERR | EPOLLRDHUP
#define MAX_EVENTS		  100

#include <map>
#include <sys/epoll.h>
#include <vector>

class ASocket;
class Connection;
class Listener;

class EpollManager
{
private:
	int epoll_fd_;

	epoll_event events_[MAX_EVENTS];

	std::map<int, ASocket*> socket_map_;

public:
	void instanceEpoll();
	void registerListenersToEpoll();
	void eventLoop();

	int	 getEpollFd() const;
	void addConnection(std::pair<int, Connection*> const& newConnection);

	EpollManager();
	EpollManager(std::vector<Listener> const& listener_vec);
	EpollManager(EpollManager const& to_copy);

	EpollManager& operator=(EpollManager const& to_copy);

	~EpollManager();
};

#endif
