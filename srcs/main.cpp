#include <csignal>
#include "csignal"
#include "epoll/EpollManager.hpp"
#include "epoll/signal.hpp"
#include "parsing/Block.hpp"
#include "socket/Listener.hpp"

void
signalHandler(int)
{
	Signal::signal = 1;
}

int
main(int ac, char** av)
{
	if (ac != 2)
		return (1);

	if (signal(SIGINT, signalHandler) == SIG_ERR)
		return 1;

	try
	{

		std::string	  buff;
		std::ifstream file(av[1]);
		Block		  block(file, Block::FILE, buff, "FILE");

		std::vector<Listener*> server_vec = block.makeServerVec();
		EpollManager		   net(server_vec);
		net.eventLoop();
	}
	catch (int s)
	{
		if (s == SIGINT)
			return 1;
		return 1;
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
