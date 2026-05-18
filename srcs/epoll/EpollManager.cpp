#include <csignal>
#include <utility>
#include <vector>

#include "epoll/EpollManager.hpp"
#include "epoll/signal.hpp"
#include "socket/ASocket.hpp"
#include "socket/Connection.hpp"
#include "socket/Listener.hpp"
#include "unistd.h"
#include "utils/utils.hpp"

void
EpollManager::addConnection(std::pair<int, Connection*> const& newConnection)
{
	socket_map_.insert(newConnection);
}

void
EpollManager::instanceEpoll()
{
	epoll_fd_ = epoll_create(2);
	if (epoll_fd_ == -1)
		throw std::logic_error(messageError("instance_epoll>epoll_create"));
}

void
EpollManager::registerListenersToEpoll()
{
	std::map<int, ASocket*>::iterator it;
	epoll_event						  ev = {};

	for (it = socket_map_.begin(); it != socket_map_.end(); it++)
	{
		int fd = it->first;
		ev.data.fd = fd;
		ev.events = EVENTS_SERVER;
		if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1)
			throw std::logic_error(
				messageError("registerListenersToEpoll>epoll_ctl"));
	}
}

void
EpollManager::eventLoop()
{
	while (true)
	{
		int nb_events = epoll_wait(epoll_fd_, events_, MAX_EVENTS, -1);
		if (Signal::signal == 1)
			throw(SIGINT);
		if (nb_events == -1)
			std::cerr << "ERROR : ";
		for (int i = 0; i < nb_events; ++i)
		{
			int fd = events_[i].data.fd;
			//! Socket cgi -> handle event (compartement different pour les cgi)
			if (socket_map_[fd]->handleEvent(*this, events_[i].events) == 1)
			{
				std::map<int, ASocket*>::iterator it = socket_map_.find(fd);
				std::cout << "The fd " << fd << " has been shut down." << '\n';
				delete it->second;
				socket_map_.erase(it);
				epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
			}
		}
	}
}

// class socket_cgi :: public Asocket
// {
// 	private:
// 	connection *connexion_;
// 	std::string response;
// 	pid_t child_status;

// }

////! Socket_cgi.handle_event()
//{

//	stock chunked par chunk la reponse
//	quand la reponse est finis (read == 0)
//	on ferme l'enfant()
//	rajoute un timeout cas d'une boucle infinie
//	call Connection->sendReponse(response) pour renvoyer la reponse au
// client
//}

int
EpollManager::getEpollFd() const
{
	return (epoll_fd_);
}

EpollManager::EpollManager() : epoll_fd_(-1), events_()
{
	instanceEpoll();
}

EpollManager::EpollManager(std::vector<Listener*> const& listener_vec) :
	epoll_fd_(-1), events_()
{
	std::vector<Listener*>::const_iterator it;

	try
	{
		for (it = listener_vec.begin(); it != listener_vec.end(); ++it)
			socket_map_.insert(
				std::make_pair((*it)->createListenerSocket(), *it));
	}
	catch (std::exception const& e)
	{
		std::vector<Listener*>::const_iterator it;
		for (it = listener_vec.begin(); it != listener_vec.end(); ++it)
			delete *it;
		throw;
	}

	instanceEpoll();
	registerListenersToEpoll();
}

EpollManager::~EpollManager()
{
	std::map<int, ASocket*>::iterator it;

	for (it = socket_map_.begin(); it != socket_map_.end(); ++it)
		delete it->second;
	close(epoll_fd_);
}
