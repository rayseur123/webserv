#include <vector>
#include "epoll/EpollManager.hpp"
#include "http/Error.hpp"
#include "parsing/Block.hpp"
#include "socket/Listener.hpp"

int
main(int ac, char** av)
{
	if (ac != 2)
		return (1);
	try
	{

		std::string buff;

		std::ifstream file(av[1]);
		Block		  block(file, Block::FILE, buff, "FILE");

		std::vector<Listener> server_vec = block.makeServerVec();
		EpollManager		  net(server_vec);
	}
	catch (Error::ErrorException const& e)
	{
		std::cerr << e.get_code();
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
