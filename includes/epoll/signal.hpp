#ifndef SIGNAL_HPP
#define SIGNAL_HPP

class Signal
{
private:
	Signal(int signal);
	Signal const& operator=(Signal const& s);

public:
	Signal(Signal const& s);
	static int signal;
	Signal();
	~Signal();
};

#endif
