#include "epoll/signal.hpp"

int Signal::signal = 0;

Signal::Signal() {};

Signal::~Signal() {};

Signal::Signal(Signal const&) {};

Signal::Signal(int) {};

Signal const&
Signal::operator=(Signal const&)
{
	return *this;
};
